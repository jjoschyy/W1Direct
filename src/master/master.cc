#include "master.h"
#include <iostream>
#include <stdio.h>
#include <string>

Master::Master(std::string* name)
	:name(*name), selectedBus(NULL)
{};


std::string* Master::GetName(void){
	return &name;
}

std::vector<Bus*>* Master::GetBuses(void){
	return &buses;
}


bool Master::HasBus(unsigned int idx){
	return buses.size() > idx;
}


Bus* Master::GetBus(unsigned int idx){
	return buses.at(idx);
}


void Master::AddBus(){
	Bus* bus = new Bus(this, (int) buses.size());
	buses.push_back(bus);
}


void Master::SetSelectedBus(Bus* bus){
	selectedBus = bus;
}


Bus* Master::GetSelectedBus(void){
	return selectedBus;
}

