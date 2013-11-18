// createkml.cc
// This program uses the KmlFactory to create a Point Placemark and
// prints the resultant KML on standard output.

#include <iostream>
#include <string>
#include "kml/dom.h"

// libkml types are in the kmldom namespace
using kmldom::CoordinatesPtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;

int main() {
  // Get the factory singleton to create KML elements.
  KmlFactory* factory = KmlFactory::GetFactory();

  // Create <coordinates>.
  CoordinatesPtr coordinates = factory->CreateCoordinates();
  // Create <coordinates>-122.0816695,37.42052549<coordinates>
  coordinates->add_latlng(37.42052549,-122.0816695);;

  // Create <Point> and give it <coordinates>.
  PointPtr point = factory->CreatePoint();
  point->set_coordinates(coordinates);  // point takes ownership

  // Create <Placemark> and give it a <name> and the <Point>.
  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_name("Cool Statue");
  placemark->set_geometry(point);  // placemark takes ownership

  // Create <kml> and give it <Placemark>.
  KmlPtr kml = factory->CreateKml();
  kml->set_feature(placemark);  // kml takes ownership.

  // Serialize to XML
  std::string xml = kmldom::SerializePretty(kml);

  // Print to stdout
  std::cout << xml;
}