// main includes
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include "SystemConfig.h"
#include "Config/Config.h"
#include "Log.h"

#include "Json/JsonReader.h"
#include "Database/DatabaseManager.h"
#include "Network/AsyncListener.hpp"
#include "Network/JsonSocket.h"

#include <iostream>
#include <thread>


boost::asio::io_service io_service;
std::atomic<bool> stopEvent;
DatabaseManager manager;
JsonReader reader;

JsonReader* GetReader() { return &reader; }
std::atomic<bool>& GetStopEvent() { return stopEvent; }

// Handle termination signals
/** Put the global variable stopEvent to 'true' if a termination signal is caught **/
void OnSignal(int s)
{
    switch (s)
    {
        case SIGINT:
        case SIGTERM:
            stopEvent = true;
            break;
#ifdef _WIN32
        case SIGBREAK:
            stopEvent = true;
            break;
#endif
    }

    io_service.stop();

    int32 timeOut = 200; // give all threads time to shut down
    while (timeOut > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        --timeOut;
    }

    signal(s, OnSignal);
}

// Define hook 'OnSignal' for all termination signals
void HookSignals()
{
    signal(SIGINT, OnSignal);
    signal(SIGTERM, OnSignal);
#ifdef _WIN32
    signal(SIGBREAK, OnSignal);
#endif
}

/// Unhook the signals before leaving
void UnhookSignals()
{
    signal(SIGINT, nullptr);
    signal(SIGTERM, nullptr);
#ifdef _WIN32
    signal(SIGBREAK, nullptr);
#endif
}

int main(int argc, char* argv[])
{
    std::string configFile, serviceParameter;

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
    ("config,c", boost::program_options::value<std::string>(&configFile)->default_value(_COMMUNCATORD_CONFIG), "configuration file")
    ("version,v", "print version and exit")
#ifdef _WIN32
    ("s", boost::program_options::value<std::string>(&serviceParameter), "<run, install, uninstall> service");
#else
    ("s", boost::program_options::value<std::string>(&serviceParameter), "<run, stop> service");
#endif

    boost::program_options::variables_map vm;

    try
    {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);
    }
    catch (boost::program_options::error const& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;

        return 1;
    }

    if (!sConfig.SetSource(configFile))
    {
        sLog.outError("Could not find configuration file %s.", configFile.c_str());
        Log::WaitBeforeContinueIfNeed();
        return 1;
    }

    sLog.Initialize();

    ///- Initialize the database connection
    if (!manager.StartDB())
    {
        Log::WaitBeforeContinueIfNeed();
        return 1;
    }

    CoreSender sender;

    sender.SetDatabaseManager(&manager);
    sender.LoadServerInfo();

    reader.SetDatabaseManager(&manager);
    reader.SetCoreSender(&sender);

    // Catch termination signals
    HookSignals();

    auto rmport = sConfig.GetIntDefault("CommunicatorServerPort", DEFAULT_COMMUNICATORSERVER_PORT);
    
    MaNGOS::AsyncListener<JsonSocket> server(io_service, rmport);
    uint32 threadCount = sConfig.GetIntDefault("NetworkThreads", 1);
    sender.SetIoService(&io_service);

    std::thread jsonThread([&]()
    {
        reader.RunParsing();
    });

    boost::thread_group threads;
    for (uint32 i = 0; i < threadCount; ++i) {
        threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
    }

    std::thread senderThread([&]()
    {
        sender.RunSender();
    });

    threads.join_all();

    senderThread.join();

    jsonThread.join();

    // Remove signal handling before leaving
    UnhookSignals();

    sLog.outString("Halting process...");
    return 0;
}