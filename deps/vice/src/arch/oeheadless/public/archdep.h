#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, KeyboardMod)
{
    KeyboardModNone   = 0,
    KeyboardModLShift = 1u << 0u,
    KeyboardModRShift = 1u << 1u,
    KeyboardModLCTRL = 1u << 2u,
    KeyboardModRCTRL = 1u << 3u,
    KeyboardModLALT = 1u << 4u,
    KeyboardModRALT = 1u << 5u,
};
