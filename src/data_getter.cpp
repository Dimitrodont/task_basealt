#include "data_getter.hpp"

size_t write_callback(char* contents, size_t size, size_t nmemb, string* data)
{
	size_t real_size = size * nmemb;
	data->append(contents, real_size);
	return real_size;
}

Data_getter::Data_getter()
{
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl == nullptr){
		cout << "FATAL ERROR curl_easy_init" << endl;
	}
}

Data_getter::~Data_getter()
{
	if(curl){
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}

string Data_getter::get_raw_data(string full_url)
{
	CURLcode res;
	string duffer = "";
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &duffer);
		
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
			cout << "curl_easy_perform return: " << curl_easy_strerror(res) << endl;
		}
	}
	return duffer;
}

int finder::get_last_pos()
{
	return last_pos;
}

string finder::get_buffer()
{
	return buffer;
}

string finder::get_raw_data()
{
	return raw_data;
}

void finder::reset(const string &raw_data)
{
	this->raw_data = raw_data;
	last_pos = 0;
	buffer = "";
}

string finder::text_between(string begin, string end)
{
	int pos1 = raw_data.find(begin, last_pos);
	if(pos1 == string::npos){	// if didnt find begin
		last_pos = 0;
		buffer = "";
		return buffer;
	}
	pos1 += begin.length();
	int pos2 = raw_data.find(end, pos1);
	if(pos2 == string::npos){	// if didnt find end
		last_pos = 0;
		buffer = "";
		return buffer;
	}
	last_pos = pos2;
	buffer = raw_data.substr(pos1, pos2 - pos1);
	return buffer;
}

string package::sum()
{
	return name + to_string(epoch) + version + release + arch + disttag + source;
}

string package::get_json()
{
	string result = "{ ";
	result += "\"name\": \"" + name + "\", ";
	result += "\"epoch\": " + to_string(epoch) + ", ";
	result += "\"version\": \"" + version + "\", ";
	result += "\"release\": \"" + release + "\", ";
	result += "\"arch\": \"" + arch + "\", ";
	result += "\"disttag\": \"" + disttag + "\", ";
	result += "\"buildtime\": " + to_string(buildtime) + ", ";
	result += "\"source\": \"" + source + "\" ";
	result += "}";
	
	return result;
}

void package::init(string raw_data)
{
	struct finder f;
	f.reset(raw_data);
	name = f.text_between("\"name\": \"", "\"");
	epoch = stoi(f.text_between("\"epoch\":", ","));
	version = f.text_between("\"version\": \"", "\"");
	release = f.text_between("\"release\": \"", "\"");
	arch = f.text_between("\"arch\": \"", "\"");
	disttag = f.text_between("\"disttag\": \"", "\"");
	buildtime = stol(f.text_between("\"buildtime\":", ","));
	source = f.text_between("\"source\": \"", "\"");
}


