/* 
 * File:   converter.h
 * Author: Schell Scivally <efsubenovex@gmail.com> = Schell Scivally
 *
 * Created on March 4, 2010, 4:47 PM
 */

#ifndef _CONVERTER_H
#define	_CONVERTER_H

#include <v8.h>
#include <string>
#include <stdint.h>

namespace converter
{
    int16_t toInt16(v8::Handle<v8::Value> const &val)
    {
        return val->IsNumber() ? static_cast<int16_t>(val->Int32Value()) : 0;
    }
    uint16_t toUint16(v8::Handle<v8::Value> const &val)
    {
        return val->IsNumber() ? static_cast<uint16_t>(val->Int32Value()) : 0;
    }
    int32_t toInt32(v8::Handle<v8::Value> const &val)
    {
        return val->IsNumber() ? static_cast<int32_t>(val->Int32Value()) : 0;
    }
    uint32_t toUint32(v8::Handle<v8::Value> const &val)
    {
        return val->IsNumber() ? static_cast<uint32_t>(val->Uint32Value()) : 0;
    }
    int64_t toInt64(v8::Handle<v8::Value> const &val)
    {
        return val->IsNumber() ? static_cast<int64_t>(val->IntegerValue()) : 0;
    }
    double toDouble(v8::Handle<v8::Value> const &val)
    {
        return val->IsNumber() ? static_cast<double>(val->NumberValue()) : 0;
    }
    bool toBool(v8::Handle<v8::Value> const &val)
    {
        return val->BooleanValue();
    }
    std::string toStdString(v8::Handle<v8::Value> const &from)
    {
        static const std::string emptyString = "<empty string>";
        v8::String::Utf8Value utf8String(from);
        const char* s = *utf8String;
        if(s)
        {
            return std::string(s, utf8String.length());
        }
        return emptyString;
    }
    
}

#endif	/* _CONVERTER_H */

