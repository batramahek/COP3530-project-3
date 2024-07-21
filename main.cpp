#include <SFML/Graphics.hpp>
#include "graph.h"
#include <unordered_map>
#include <iostream>
#include <unordered_map>
#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#define pi 3.14159265358979323846

//Earth Radius in Kilometers found online
const double EarthR = 6378.0;

double Formula(double Latitude_1, double Longitute_1, double Latitude_2, double Longitute_2){
    double Longitude = (Latitude_2 - Latitude_1) * pi / 180.0;
    double Latitude = (Longitute_2 - Longitute_1) * pi / 180.0;

    Latitude_1 = Latitude_1 * pi / 180.0;
    Latitude_2 = Latitude_2 * pi / 180.0;

    double Result = sin(Latitude/2) * sin(Latitude/2) + cos(Latitude_1) * cos(Latitude_2) * sin(Longitude/2) * sin(Longitude/2); 
    double Result_1 = 2 * atan2(sqrt(a), sqrt(1-a));
    double final = EarthR * Result_1;
    return final;
}

class Drawer : public osmium::draw::draw{
    public:
        AdjancencyList& graph;
        int Node_Total = 0;

        cosnt int Node_Max = 100000;
        Drawer(AdjancencyList& list) : graph(list) {}

        void nodes(cosnt osmium::Node& nodes){
            if (Node_Total < Node_Max) {
                graph.Node_Insertion(node.id(), node.location.Lat(), node.location.Long());
                Node_Total++;
            }
        }

        void Direction (const osmium::Direction& Direct){
            const auto& Direction_of_Node = Direct.
        }
}


int main (){
    sf::RenderWindow window(sf::VideoMode(800, 600), "Project 3");
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEven(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        /* This will show the actual graph in the window, set up later
        window.draw(example);
        */
        window.dislplay();
    }
    return 0;
}