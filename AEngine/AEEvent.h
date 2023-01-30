#pragma once
#ifndef AE_EVENT_H
#define AE_EVENT_H


#include <iostream>
#include <vector>


#include <SDL.h>
#include <SDL_opengles2.h>

#include "Helpers.h"

template <typename Targ>
class EventCallBack //Un sistema de callback para hacer llamadas a funciones.
{
private:
    std::vector<void (*)(Targ)> _functions;

public:
    void AddFunction(void (*func)(Targ))
    {
        _functions.push_back(func);
    }

    void RemoveFunction(void (*func)(Targ))
    {

        VECTOR_REMOVE(_functions, func);
       // _functions.erase(std::remove(_functions.begin(), _functions.end(), func), _functions.end());
    }

    void Invoke(Targ& arg)
    {
        for (auto func : _functions)
            func(arg);
    }
};

class Application
{
public:
    //Eventos propios de el motor
	EventCallBack<int> OnAppInit, OnAppUpdate,OnAppFixedUpdate, OnAppEnd;
    EventCallBack<SDL_Keycode> OnKeyDown, OnKeyUp, OnKey;
    EventCallBack<int> OnMouseDown, OnMouseUp, OnMouseMove;
    int s = 5;
    int a = 2;

	void CallOnInit(){ OnAppInit.Invoke(a); } //Funcion que se llama cuando se inicia la app
	void CallOnAppUpdate() { OnAppUpdate.Invoke(s); }; //Funcion que se llama cuando se actualiza la app
	void CallOnAppFixedUpdate() { OnAppFixedUpdate.Invoke(s); }; //Funcion que se llama despues del renderizado
	void CallOnAppEnd() { OnAppEnd.Invoke(s); }; //Funcion que se llama cuando de cierra la app
   
    void CallOnKeyPressed(SDL_Keycode key, int type = 0)
    {
        switch (type)
        {
        case 0:
            OnKeyDown.Invoke(key);
        case 1:
            OnKeyUp.Invoke(key);
        case 2:
            OnKey.Invoke(key);
        default:
            break;
        }
    };
    void CallOnMousePressed(int key, int type = 0)
    {
        switch (type)
        {
        case 0:
            OnMouseDown.Invoke(key);
        case 1:
            OnMouseUp.Invoke(key);
        case 2:
            OnMouseMove.Invoke(key);
        default:
            break;
        }
    };

};

#endif // !AE_EVENT_H