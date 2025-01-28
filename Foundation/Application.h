//
// Created by davyxu on 2025/1/28.
//

#pragma once


class IApplication {
public:
    virtual void Init() = 0;

    virtual void Exit() = 0;
};


IApplication *GetApplication();