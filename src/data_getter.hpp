#pragma once

#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace std;

size_t write_callback(char* contents, size_t size, size_t nmemb, string* data);

class Data_getter
{
public:
	Data_getter();
	string get_raw_data(string full_url);
	~Data_getter();
	
private:
	CURL* curl = nullptr;
};

struct finder
{
public:
	int get_last_pos();
	string get_buffer();
	string get_raw_data();
	void reset(const string &raw_data);
	string text_between(string begin, string end);
private:
	int last_pos = 0;
	string raw_data = "";
	string buffer = "";
};

struct package
{
	string 	name 		= "";
	int 	epoch		= 0 ;
	string 	version		= "";
	string 	release		= "";
	string 	arch		= "";
	string 	disttag		= "";
	long 	buildtime	= 0 ;
	string 	source		= "";
	
	string sum(); // I do not know how to compare them, no exact instructions
	string get_json();
	void init(string raw_data);
};

