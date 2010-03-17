//c++///////////////////////////////////////////////////////////////////////////
// Copyleft Schell Scivally Enterprise - Civilian Software
// All rights reversed
////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <vector>
#include <iostream>
#include <exception>
#include <v8.h>
#include <node.h>
#include <node_events.h>
#include <converter.h>
#include <RtAudio.h>
#include <RtError.h>

using namespace v8;
using namespace converter;
//--------------------------------------
//  FILE WIDE VARS
//--------------------------------------
/**
 *  Holds our string name apis.
 */
std::map<int, std::string> apiMap;
std::map<std::string, int> invertApiMap;
/**
 *  Holds our template for RtAudio.
 */
static Persistent<ObjectTemplate> rtaudio_template;

//--------------------------------------
//  FILE WIDE FUNCTIONS
//--------------------------------------
/**
 *  Copies a DeviceInfo struct to DeviceInfo js object
 *
 *  @return Handle<Array>
 */
Local<Object> deviceInfoTo(RtAudio::DeviceInfo &info)
{
    HandleScope scope;
    Local<Object> device = Object::New();

    device->Set(String::New("probed"), Boolean::New(info.probed));
    device->Set(String::New("name"), String::New(info.name.c_str()));
    device->Set(String::New("outputChannels"), Integer::New(info.outputChannels));
    device->Set(String::New("inputChannels"), Integer::New(info.inputChannels));
    device->Set(String::New("duplexChannels"), Integer::New(info.duplexChannels));
    device->Set(String::New("isDefaultOutput"), Boolean::New(info.isDefaultOutput));
    device->Set(String::New("isDefaultInput"), Boolean::New(info.isDefaultInput));
    int numSampleRates = info.sampleRates.size();
    Local<Array> sampleRates = Array::New(numSampleRates);
    for(int j = 0; j < numSampleRates; j++)
    {
        sampleRates->Set(Integer::New(j), Int32::New(info.sampleRates.at(j)));
    }
    device->Set(String::New("sampleRates"), sampleRates);
    return scope.Close(device);
}
/**
 *  Copies a DeviceInfo js object to DeviceInfo struct
 *
 *  @return Handle<Array>
 */
RtAudio::DeviceInfo deviceInfoFrom(Local<Object> device)
{
    HandleScope scope;
    RtAudio::DeviceInfo info;
    info.probed = device->Get(String::New("probed"))->BooleanValue();
    info.name   = toStdString(device->Get(String::New("name")));
    info.outputChannels = device->Get(String::New("outputChannels"))->IntegerValue();
    info.inputChannels  = device->Get(String::New("inputChannels"))->IntegerValue();
    info.duplexChannels = device->Get(String::New("duplexChannels"))->IntegerValue();
    info.isDefaultOutput = device->Get(String::New("isDefaultOutput"))->IntegerValue();
    info.isDefaultInput = device->Get(String::New("isDefaultInput"))->BooleanValue();
    Local<Array> sampleRates(Array::Cast(*device->Get(String::New("sampleRates"))));
    std::vector<unsigned int> infoRates;
    for(int i = 0; i < sampleRates->Length(); i++)
    {
        infoRates.push_back(sampleRates->Get(Integer::New(i))->Int32Value());
    }
    info.sampleRates = infoRates;
    
    return info;
}
/**
 *  Copies a StreamParameters struct to js object
 *
 *  @return Local<Object>
 */
Local<Object> streamParametersTo(RtAudio::StreamParameters& params)
{
    HandleScope scope;
    Local<Object> to = Object::New();

    to->Set(String::New("deviceId"), Boolean::New(params.deviceId));
    to->Set(String::New("nChannels"), Uint32::New(params.nChannels));
    to->Set(String::New("firstChannel"), Uint32::New(params.firstChannel));

    return scope.Close(to);
}
/**
 *  Copies a js object to StreamParameters struct
 *
 *  @return Handle<Array>
 */
RtAudio::StreamParameters streamParametersFrom(Local<Object> from)
{
    RtAudio::StreamParameters params;
    params.deviceId = toBool(from->Get(String::New("probed")));
    params.nChannels = toUint32(from->Get(String::New("nChannels")));
    params.firstChannel = toUint32(from->Get(String::New("outputChannels")));
    
    return params;
}
/**
 *  Makes a js object from RtAudioStreamFlags.
 *
 *  @return Handle<Value>
 */
Handle<Value> streamFlagsTo(RtAudioStreamFlags& flags)
{
    Local<Object> to = Object::New();
    to->Set(String::New("nonInterleaved"), Boolean::New(flags & RTAUDIO_NONINTERLEAVED));
    to->Set(String::New("minimizeLatency"), Boolean::New(flags & RTAUDIO_MINIMIZE_LATENCY));
    to->Set(String::New("hogDevice"), Boolean::New(flags & RTAUDIO_HOG_DEVICE));
    return to;
}
/**
 *  Makes RtAudioStreamFlags from an object.
 *
 *  @return RtAudioStreamFlags
 */
RtAudioStreamFlags streamFlagsFrom(Local<Object> from)
{
    RtAudioStreamFlags flags = 0;
    bool nonInterleaved = toBool(from->Get(String::New("nonInterleaved")));
    bool minimizeLatency = toBool(from->Get(String::New("minimizeLatency")));
    bool hogDevice = toBool(from->Get(String::New("hogDevice")));

    if(nonInterleaved)
        flags |= RTAUDIO_NONINTERLEAVED;
    if(minimizeLatency)
        flags |= RTAUDIO_MINIMIZE_LATENCY;
    if(hogDevice)
        flags |= RTAUDIO_HOG_DEVICE;
    return flags;
}
/**
 *  Copies a StreamOptions struct to js object.detail
 *
 *  @return Handle<Value>
 */
Handle<Value> streamOptionsTo(RtAudio::StreamOptions& options)
{
    HandleScope scope;
    Local<Object> to = Object::New();
    to->Set(String::New("flags"), streamFlagsTo(options.flags));
    to->Set(String::New("numberOfBuffers"), Uint32::New(options.numberOfBuffers));
    to->Set(String::New("streamName"), String::New(options.streamName.c_str()));
    to->Set(String::New("priority"), Integer::New(options.priority));
    return scope.Close(to);
}
/**
 *  Copies a js object to StreamOptions struct
 *
 *  @return RtAudio::StreamOptions
 */
RtAudio::StreamOptions streamOptionsFrom(Local<Object> from)
{
    RtAudio::StreamOptions options;
    options.flags = streamFlagsFrom(Object::Cast(*from->Get(String::New("flags"))));
    options.numberOfBuffers = toUint32(from->Get(String::New("numberOfBuffers")));
    options.streamName = toStdString(from->Get(String::New("streamName")));
    options.priority = toInt32(from->Get(String::New("priority")));
    return options;
}
/**
 *  Copies a js string to RtAudioFormat
 *
 *  @return RtAudioFormat
 */
RtAudioFormat formatFrom(Local<String> from)
{
    std::string format = toStdString(from);
    if(format == "sInt8")
        return RTAUDIO_SINT8;
    if(format == "sInt16")
        return RTAUDIO_SINT16;
    if(format == "sInt32")
        return RTAUDIO_SINT32;
    if(format == "float32")
        return RTAUDIO_FLOAT32;
    if(format == "float64")
        return RTAUDIO_FLOAT64;

    return RTAUDIO_FLOAT32; // default
}
/**
 *  Returns the compiled apis.
 *
 *  @return Array
 */
Handle<Value> getCompiledApis(const Arguments& args)
{
    // shows what apis were compiled into rtaudio 
    // (i've only compiled in core audio in librtaudio)
    std::vector<RtAudio::Api> apis;
    RtAudio::getCompiledApi(apis);

    // create and set an array in js to hold the compiled apis
    Local<Array> compiledApis = Array::New(apis.size());
    for(int i = 0; i < apis.size(); i++)
    {
        compiledApis->Set(Integer::New(i), String::New(apiMap[apis.at(i)].c_str()));
    }

    return compiledApis;
}

/**
 *  Gets the probed value of a DeviceInfo object.
 *
 *  @return Boolean
 */
Handle<Value> enumerateDevices(const Arguments& args)
{
    HandleScope scope;
    RtAudio audio;

    int deviceCount = audio.getDeviceCount();
    RtAudio::DeviceInfo info;
    Local<Array> devices = Array::New(deviceCount);
    for(int i = 0; i < deviceCount; i++)
    {
        info = audio.getDeviceInfo(i);
        Local<Object> device = deviceInfoTo(info);
        // add this device to the list of devices
        devices->Set(Integer::New(i), device);
    }

    return scope.Close(devices);
}
/**
 *  Unwraps an rtaudio instance from a wrapper object.
 *
 *  @return RtAudio*
 */
RtAudio* unwrapRtAudio(Handle<Object> obj)
{
    Handle<External> intField = Handle<External>::Cast(obj->GetInternalField(0));
    void* ptr = intField->Value();
    return static_cast<RtAudio*>(ptr);
}
/**
 *  Wraps RtAudio::getCurrentApi.
 *
 *  @return a string specifying the api used currently.
 */
Handle<Value> getCurrentApi(const Arguments& args)
{
    HandleScope scope;
    RtAudio* audio = unwrapRtAudio(args.Holder());
    Local<String> api = String::New(apiMap[audio->getCurrentApi()].c_str());
    return api;
}
/**
 *  Wraps RtAudio::getDeviceCount.
 *
 *  @return int number of devices connected
 */
Handle<Value> getDeviceCount(const Arguments& args)
{
    HandleScope scope;
    RtAudio* audio = unwrapRtAudio(args.Holder());
    Local<Integer> n = Integer::NewFromUnsigned(audio->getDeviceCount());
    return n;
}
/**
 *  Wraps RtAudio::getDeviceInfo.
 *
 *  @return a device info object
 */
Handle<Value> getDeviceInfo(const Arguments& args)
{
    HandleScope scope;
    RtAudio* audio = unwrapRtAudio(args.Holder());
    uint32_t deviceNumber = toUint32(args[0]);
    RtAudio::DeviceInfo info = audio->getDeviceInfo(deviceNumber);
    return deviceInfoTo(info);
}
/**
 *  Wraps RtAudio::getDefaultOutputDevice.
 *
 *  @return Number
 */
Handle<Value> getDefaultOutputDevice(const Arguments& args)
{
    HandleScope scope;
    RtAudio* audio = unwrapRtAudio(args.Holder());
    uint32_t device = audio->getDefaultOutputDevice();
    return Uint32::New(device);
}
/**
 *  Wraps RtAudio::getDefaultInputDevice.
 *
 *  @return Number
 */
Handle<Value> getDefaultInputDevice(const Arguments& args)
{
    HandleScope scope;
    RtAudio* audio = unwrapRtAudio(args.Holder());
    uint32_t device = audio->getDefaultInputDevice();
    return Uint32::New(device);
}
/**
 *  Wraps RtAudio::openStream.
 *
 *  @return Undefined
 */
Handle<Value> openStream(const Arguments& args)
{
    RtAudio* audio = unwrapRtAudio(args.Holder());
    RtAudio::StreamParameters outputParameters = streamParametersFrom(Object::Cast(*args[0]));
    RtAudio::StreamParameters inputParameters = streamParametersFrom(Object::Cast(*args[1]));
    RtAudioFormat format = formatFrom(String::Cast(*args[2]));
    unsigned int sampleRate = toUint32(args[3]);
    
}
/**
 *  A to string function.
 *
 *  @return a string
 */
Handle<Value> toString(const Arguments& args)
{
    return String::New("[native RtAudio]");
}
/**
 *  Creates an object template for rtaudio.
 */
Handle<ObjectTemplate> getRtAudioTemplate()
{
    HandleScope scope;
    // create the object template for our js version of RtAudio
    Local<ObjectTemplate> tmplt = ObjectTemplate::New();
    tmplt->SetInternalFieldCount(1);
    // right now we're only exposing two functions, we'll add to this later
    tmplt->Set(String::New("getCurrentApi"), FunctionTemplate::New(getCurrentApi));
    tmplt->Set(String::New("getDeviceCount"), FunctionTemplate::New(getDeviceCount));
    tmplt->Set(String::New("getDeviceInfo"), FunctionTemplate::New(getDeviceInfo));
    tmplt->Set(String::New("getDefaultOutputDevice"), FunctionTemplate::New(getDefaultOutputDevice));
    tmplt->Set(String::New("getDefaultInputDevice"), FunctionTemplate::New(getDefaultInputDevice));
    tmplt->Set(String::New("toString"), FunctionTemplate::New(toString));
    return scope.Close(tmplt);
}
/**
 *  Creates a new RtAudio instance to track through js.
 *  Where do we destroy these objects? How is that handled in v8?
 *
 *  @return Handle<Object> a new javascriptable rtaudio instance
 */
Handle<Value> NewRtAudio(const Arguments& args)
{
    HandleScope scope;
    RtAudio *audio;
    // this doesn't work yet, but we should
    // allow the user to choose which api they
    // would like to use
    if(false/*args.Length() > 0*/)
    {
        //// if the user gave us an api to use,
        //// use that
        // Handle<Value> arg = args[0];
        // String::Utf8Value value(arg);
        // RtAudio::Api api = invertApiMap[value];
        // audio = new RtAudio(api);
    }
    else
    {
        audio = new RtAudio();
    }

    // if we have not created a template for rtaudio, do so
    if(rtaudio_template.IsEmpty())
    {
        Handle<ObjectTemplate> tmplt = getRtAudioTemplate();
        rtaudio_template = Persistent<ObjectTemplate>::New(tmplt);
    }
    Handle<ObjectTemplate> rtaudioTemplate = rtaudio_template;

    //create an empty rtaudio wrapper
    Handle<Object> wrapper = rtaudioTemplate->NewInstance();

    // wrap the raw c++ pointer in an external so it can be
    // referenced in js
    Handle<External> externalAudio = External::New(audio);

    // store the rtaudio pointer in the js wrapper
    wrapper->SetInternalField(0, externalAudio);

    // return the result through the current handle scope.  since each
    // of these handles will go away when the handle scope is deleted
    // we need to call Close to let one, our wrapper, escape into the
    // outer handle scope
    return scope.Close(wrapper);
}

//--------------------------------------
//  TESTS
//--------------------------------------

//--------------------------------------
//  INIT HOOK
//--------------------------------------
/**
 *  The init function.
 *  node passes us a reference to what is returned by the 'require' function
 *  in js as 'target'.
 */
extern "C" void init (Handle<Object> target)
{
    HandleScope scope;

    // poplate the apiMap
    apiMap[RtAudio::UNSPECIFIED]    = "unspecified";
    apiMap[RtAudio::MACOSX_CORE]    = "OS X Core Audio";
    apiMap[RtAudio::WINDOWS_ASIO]   = "Windows ASIO";
    apiMap[RtAudio::WINDOWS_DS]     = "Windows Direct Sound";
    apiMap[RtAudio::UNIX_JACK]      = "Jack Client";
    apiMap[RtAudio::LINUX_ALSA]     = "Linux ALSA";
    apiMap[RtAudio::LINUX_OSS]      = "Linux OSS";
    apiMap[RtAudio::RTAUDIO_DUMMY]  = "RtAudio Dummy";
    // populate the inverted api map
    invertApiMap[apiMap[RtAudio::MACOSX_CORE]]      = RtAudio::MACOSX_CORE;
    invertApiMap[apiMap[RtAudio::WINDOWS_ASIO]]     = RtAudio::WINDOWS_ASIO;
    invertApiMap[apiMap[RtAudio::WINDOWS_DS]]       = RtAudio::WINDOWS_DS;
    invertApiMap[apiMap[RtAudio::UNIX_JACK]]        = RtAudio::UNIX_JACK;
    invertApiMap[apiMap[RtAudio::LINUX_ALSA]]       = RtAudio::LINUX_ALSA;
    invertApiMap[apiMap[RtAudio::LINUX_OSS]]        = RtAudio::LINUX_OSS;
    invertApiMap[apiMap[RtAudio::RTAUDIO_DUMMY]]    = RtAudio::RTAUDIO_DUMMY;

    RtAudio audio;

    // set the 'numDevices' in js
    unsigned int numDevices = audio.getDeviceCount();
    target->Set(String::New("numDevices"), Number::New((double) numDevices));

    // expose getCompiledApis
    target->Set(String::New("getCompiledApis"), FunctionTemplate::New(getCompiledApis)->GetFunction());
    // expose a function for enumerating all the hardware's devices
    target->Set(String::New("enumerateDevices"), FunctionTemplate::New(enumerateDevices)->GetFunction());
    // expose a function for creating new RtAudio instances
    target->Set(String::New("New"), FunctionTemplate::New(NewRtAudio)->GetFunction());
    // expose the api map
    printf("%i apis listed\n", (int) apiMap.size());
    Local<Array> toExternalApiMap = Array::New(apiMap.size());
    for(int i = 0; i < apiMap.size(); i++)
    {
        printf("    %i\n", i);
        try
        {
            toExternalApiMap->Set(Integer::New(i), String::New(apiMap.at(i).c_str()));
        }
        catch(std::exception& e)
        {
            printf("rtaudio_binding::init() ERROR -\n    %s", e.what());
            e.what();
        }
    }
    target->Set(String::New("apiMap"), toExternalApiMap);

}