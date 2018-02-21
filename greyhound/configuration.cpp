#include <greyhound/configuration.hpp>

#include <entwine/third/arbiter/arbiter.hpp>
#include <entwine/util/json.hpp>

namespace greyhound
{

namespace
{

Json::Value defaults()
{
    Json::Value json;
    json["cacheSize"] = "200MB";
    json["paths"] = entwine::toJsonArray(
            std::vector<std::string>{
                "/greyhound", "~/greyhound",
                "/entwine", "~/entwine",
                "/opt/data"
            });
    json["tmp"] = entwine::arbiter::fs::getTempPath();
    json["resourceTimeoutMinutes"] = 2;
    json["http"]["port"] = 8080;

    Json::Value headers;
    headers["Cache-Control"] = "public, max-age=300";
    headers["Access-Control-Allow-Origin"] = "*";
    headers["Access-Control-Allow-Methods"] = "GET,PUT,POST,DELETE";
    json["http"]["headers"] = headers;

    return json;
}

Configuration::Args normalize(int argc, char** argv)
{
    Configuration::Args args;
    for (int i(1); i < argc; ++i)
    {
        std::string arg(argv[i]);

        if (arg.size() > 2 && arg.front() == '-' && std::isalpha(arg[1]))
        {
            // Expand args of the format "-xvalue" to "-x value".
            args.push_back(arg.substr(0, 2));
            args.push_back(arg.substr(2));
        }
        else
        {
            args.push_back(argv[i]);
        }
    }
    return args;
}

} // unnamed namespace

Configuration::Configuration(const int argc, char** argv)
    : m_json(parse(normalize(argc, argv)))
{ }

Json::Value Configuration::parse(const Args& args)
{
    return fromArgs(fromFile(args), args);
}

Json::Value Configuration::fromFile(const Args& args)
{
    bool configFlag(false);
    std::string configPath;

    for (const auto a : args)
    {
        if (a == "-c") configFlag = true;
        else if (configFlag)
        {
            if (a.front() != '-') configPath = a;
            else configFlag = false;
        }
    }

    Json::Value config(defaults());

    if (configPath.size())
    {
        std::cout << "Using configuration at " << configPath << std::endl;
        entwine::recMerge(
                config,
                entwine::parse(entwine::arbiter::Arbiter().get(configPath)));
    }
    else
    {
        std::cout << "Using default config" << std::endl;
    }

    return config;
}

Json::Value Configuration::fromArgs(Json::Value json, const Args& args)
{
    std::string flag;
    for (const auto a : args)
    {
        if (a == "-w") { json["allowWrite"] = true; flag = ""; }
        else if (a.front() == '-') flag = a;
        else if (flag == "-c") { /* Already handled the config-path flag. */ }
        else if (flag == "-p") json["http"]["port"] = std::stoi(a);
        else if (flag == "-s") json["http"]["securePort"] = std::stoi(a);
        else if (flag == "-k") json["http"]["keyFile"] = a;
        else if (flag == "-c") json["http"]["certFile"] = a;
        else if (flag == "-d") json["paths"].append(a);
        else if (flag == "-a") json["tmp"] = a;
        else std::cout << "Ignored argument: " << a << std::endl;
    }
    return json;
}

} // namespace greyhound

