#pragma once

#include "Libs/imgui.h"
#include <vector>
#include <utility>
#include "Vector2.hpp"

#ifndef NODE_H
#define NODE_H

template <typename T>
struct Socket
{
    T type_socket;
    Node* sockets_in;
    Node* sockets_out;
};


class Node {
public:
    int id;
    Vector2 position_canvas;
    Vector2 handle_position;
    Vector2 handle_size;
    bool selected;
    
    std::vector<Socket<T>> sockets_in;
    std::vector<Socket<>> sockets_out;


    Node(int id_, Vector2 pos_,bool selected_=false) :
        id(id_), handle_position(pos_), selected(selected_) {}

    void Draw() {
        ImGui::SetCursorScreenPos(handle_position.toIgV2());
        ImGui::BeginGroup();
        ImGui::Text("Node %d", id);
        ImGui::EndGroup();
    }

    bool operator==(const Node& other) const {
        return id == other.id;
    }
    bool operator!=(const Node& other) const {
        return id != other.id;
    }

    virtual std::string GetLineCode() const;

private:


};


#endif // !NODE_H
