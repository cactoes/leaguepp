#pragma once

#ifndef __GENERAL_HPP__
#define __GENERAL_HPP__

// ~~ general utils

bool IsDebug() {
#ifdef _DEBUG
    return true;
#else
    return false;
#endif
}

#endif // __GENERAL_HPP__