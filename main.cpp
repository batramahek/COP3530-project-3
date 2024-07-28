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

//formula to calculate weights based on longitude and latitude
double Formula(double Latitude_1, double Longitude_1, double Latitude_2, double Longitude_2) {
    double Longitude = (Latitude_2 - Latitude_1) * pi / 180.0;
    double Latitude = (Longitude_2 - Longitude_1) * pi / 180.0;

    Latitude_1 = Latitude_1 * pi / 180.0;
    Latitude_2 = Latitude_2 * pi / 180.0;

    double Result = sin(Latitude / 2) * sin(Latitude / 2) + cos(Latitude_1) * cos(Latitude_2) * sin(Longitude / 2) * sin(Longitude / 2);
    double Result_1 = 2 * atan2(sqrt(Result), sqrt(1 - Result));
    double final = EarthR * Result_1;
    return final;
}

class Drawer : public osmium::handler::Handler {
public:
        AdjacencyList& graph;
    int Node_Total = 0;
    int nodes_parsed = 0;

    
    const int Node_Max = 200000;
    Drawer(AdjacencyList& list) : graph(list) {}

    //override node function
    void node(const osmium::Node& nodes) {
        //parse 200k nodes and insert every other node
        if (Node_Total < Node_Max && Node_Total <= 100000) {
            if (nodes_parsed % 2  == 0)
            {
                graph.insertNode(nodes.id(), nodes.location().lat(), nodes.location().lon());
                Node_Total++;
            }
            nodes_parsed++;

            cout << "Node ID: " << nodes.id() << "\t" << Node_Total << "\t";
        }
    }


    //override way function
    void way(const osmium::Way& Direct) {
        const osmium::NodeRefList& direction_of_nodes = Direct.nodes();

        for (auto it = direction_of_nodes.begin(); it != direction_of_nodes.end(); it++) {
            auto it2 = std::next(it);
            if (it2 != direction_of_nodes.end()) {
                int fromID = it->ref();
                int toID = it2->ref();

                // Retrieve the locations from the graph
                auto from = graph.getNodeLoc().find(fromID);
                auto to = graph.getNodeLoc().find(toID);

                if (from != graph.getNodeLoc().end() && to != graph.getNodeLoc().end()) {
                    double lat1 = from->second.first;
                    double lon1 = from->second.second;
                    double lat2 = to->second.first;
                    double lon2 = to->second.second;

                    double dist = Formula(lat1, lon1, lat2, lon2);
                    graph.insertEdge(fromID, toID, dist);
                }

            }
        }

    }
};

//function to load OSM data
void OSMDATA(const string& name, AdjacencyList& graph) {
    osmium::io::Reader reader(name);
    Drawer handler(graph);
    osmium::apply(reader, handler);
    reader.close();
}

void GraphVisual(sf::RenderWindow& window, AdjacencyList& graph) {
    sf::CircleShape nodeShape(0.1);
    nodeShape.setFillColor(sf::Color::Blue);
    sf::VertexArray edges(sf::Lines);

    // Calculate min and max latitude and longitude for normalization
    double minLat = std::numeric_limits<double>::max();
    double maxLat = std::numeric_limits<double>::lowest();
    double minLon = std::numeric_limits<double>::max();
    double maxLon = std::numeric_limits<double>::lowest();

    for (const auto& node : graph.getNodeLoc()) {
        double lat = node.second.first;
        double lon = node.second.second;
        if (lat < minLat) minLat = lat;
        if (lat > maxLat) maxLat = lat;
        if (lon < minLon) minLon = lon;
        if (lon > maxLon) maxLon = lon;
    }

    // Calculate scale factors
    double latRange = maxLat - minLat;
    double lonRange = maxLon - minLon;

    // Ensure we maintain the aspect ratio
    double scale = std::min(window.getSize().x / lonRange, window.getSize().y / latRange);



    for (const auto& couple : graph.getAdjacencyList()) {
        int Node_1 = couple.first;
        auto it1 = graph.getNodeLoc().equal_range(Node_1);
        if (it1.first != it1.second) {
            double Latitude_1 = it1.first->second.first;
            double Longitude_1 = it1.first->second.second;

            for (const auto& neighbors : couple.second) {
                int Node_2 = neighbors.first;
                auto it2 = graph.getNodeLoc().equal_range(Node_2);
                if (it2.first != it2.second) {
                    double Latitude_2 = it2.first->second.first;
                    double Longitude_2 = it2.first->second.second;

                    // Normalize positions and scale
                    double x1 = (Longitude_1 - minLon) * scale;
                    double y1 = (Latitude_1 - minLat) * scale;
                    double x2 = (Longitude_2 - minLon) * scale;
                    double y2 = (Latitude_2 - minLat) * scale;



                    edges.append(sf::Vertex(sf::Vector2f(x1, y1), sf::Color::White));
                    edges.append(sf::Vertex(sf::Vector2f(x2, y2), sf::Color::White));
                }
            }
        }
    }
    window.draw(edges);

    for (const auto& node : graph.getNodeLoc()) {
        double Latitude = node.second.first;
        double Longitude = node.second.second;

        // Normalize positions and scale
        double x = (Longitude - minLon) * scale;
        double y = (Latitude - minLat) * scale;



        nodeShape.setPosition(x, y);
        window.draw(nodeShape);
    }
}


int main()
{
    //store osm file name and pass through function to load data
    string osm_filename = "florida-latest.osm.pbf";
    AdjacencyList graph;
    cout << "loading data in" << endl;
    OSMDATA(osm_filename, graph);

    cout << "loaded data, now opening window" << endl;

    sf::RenderWindow window(sf::VideoMode(1500, 1200), "Project 3");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        GraphVisual(window, graph);
        window.display();
    }
    return 0;
}