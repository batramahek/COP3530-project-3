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

double Formula(double Latitude_1, double Longitude_1, double Latitude_2, double Longitude_2){
    double Longitude = (Latitude_2 - Latitude_1) * pi / 180.0;
    double Latitude = (Longitude_2 - Longitude_1) * pi / 180.0;

    Latitude_1 = Latitude_1 * pi / 180.0;
    Latitude_2 = Latitude_2 * pi / 180.0;

    double Result = sin(Latitude/2) * sin(Latitude/2) + cos(Latitude_1) * cos(Latitude_2) * sin(Longitude/2) * sin(Longitude/2); 
    double Result_1 = 2 * atan2(sqrt(Result), sqrt(1-Result));
    double final = EarthR * Result_1;
    return final;
}

class Drawer : public osmium::handler::Handler{
    public:
        AdjacencyList& graph;
        int Node_Total = 0;

        const int Node_Max = 100000;
        Drawer(AdjancencyList& list) : graph(list) {}

        void nodes(const osmium::Node& nodes){
            if (Node_Total < Node_Max) {
                graph.insertNode(nodes.id(), nodes.location.Lat(), nodes.location.Long());
                Node_Total++;
            }
        }

        void Direction (const osmium::Way& Direct){
            const auto& Direction_of_Node = Direct.nodes();
            for (size_t x = 1; x, Direction_of_Node.size(); x++){
                auto Node_1 = Direction_of_Node[x-1].ref();
                auto Node_2 = Direction_of_Node[x].ref();
                if (graph.getNodeLoc().count(Node_1)&&graph.getNodeLoc().count(Node_2))
                {
                    double Latitude_1 = graph.getNodeLoc().at(Node_1).first;
                    double Longitude_1 = graph.getNodeLoc().at(Node_1).second;
                    double Latitude_2 = graph.getNodeLoc().at(Node_2).first;
                    double Longitude_2 = graph.getNodeLoc().at(Node_2).second;
                    double Dist = Formula(Latitude_1, Longitude_1, Latitude_2, Longitude_2);
                    graph.insertEdge(Node_1, Node_2, Dist);
                }
            }
        }
};

void OSMDATA(const string& name, AdjacencyList& graph){
    osmium::io::Reader reader(name);
    Drawer handler(graph);
    osmium::apply(reader, handler);
    reader.close();
}

void GraphVisual(sf::RenderWindow& window, const AdjacencyList& graph){
    sf::CircleShape nodeShape(2);
    nodeShape.setFillColor(sf::Color::Blue);
    sf::VertexArray edges(sf::Lines);
    for (const auto& couple : graph.getAdjList()) {
        int Node_1 = couple.first;
        double Latitude_1 = graph.getNodeLoc().at(Node_1).first;
        double Longitude_1 = graph.getNodeLoc().at(Node_1).second;
        for(const auto& neighbors : couple.second) {
            int Node_2 = neighbors.first;
            double Latitude_2 = graph.getNodeLoc().at(Node_2).first;
            double Longitude_2 = graph.getNodeLoc().at(Node_2).second;
            sf::Vector2f pos1(Longitude_1 * 1000, Latitude_1 * 1000);
            sf::Vector2f pos2(Longitude_2 * 1000, Latitude_2 * 1000);
            edges.append(sf::Vertex(pos1, sf::Color::White));
            edges.append(sf::Vertex(pos1, sf::Color::White));
        }
    }
    window.draw(edges);
    for(const auto& node : graph.getNodeLoc()){
        double Latitude = node.second.first;
        double Longitude = node.second.second;
        nodeShape.setPosition(Longitude * 1000, Latitude * 1000);
        window.draw(nodeShape);
    }
}


int main (){
    string osm_filename = "florida-latest.osm.pbf";
    AdjacencyList graph;
    OSMDATA(osm_filename, graph);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Project 3");
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        GraphVisual(window, graph);
        window.dislplay();
    }
    return 0;
}