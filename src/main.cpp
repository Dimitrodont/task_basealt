#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "data_getter.hpp"

// https://rdb.altlinux.org/api/export/branch_binary_packages/
// sisyphus
// p10

long string_to_ver(string str);
int num_or_bullshit(string str);

int main()
{
	string target_url = "https://rdb.altlinux.org/api/export/branch_binary_packages/";
	
	Data_getter dg;
	struct finder fndr;
	struct package temp_pack;
	map<string, package> branch1;
	map<string, package> branch2;
	vector<package> vec1;	// branch1 - branch2
	vector<package> vec2;	// branch2 - branch1
	vector<package> vec3;	// max versions from branch1 over than in branch2

	
	
// get branch1
	string branch1_name = "";
	cout << "Please enter the name of the first branch: ";
	cin >> branch1_name;
	cout << "The download of the first branch has started, please wait..." << endl;
	fndr.reset(dg.get_raw_data(target_url + branch1_name));
	cout << "Branch " << branch1_name << " length: " << fndr.text_between("\"length\":", ",") << endl;
	while(fndr.text_between("{", "}") != ""){
		temp_pack.init(fndr.get_buffer());
		branch1.insert(map<string, package>::value_type(temp_pack.sum(), temp_pack));
	}
	
	
	
// get branch2
	string branch2_name = "";
	cout << "Please enter the name of the second branch: ";
	cin >> branch2_name;
	cout << "The download of the second branch has started, please wait..." << endl;
	fndr.reset(dg.get_raw_data(target_url + branch2_name));
	cout << "Branch " << branch1_name << " length: " << fndr.text_between("\"length\":", ",") << endl;
	while(fndr.text_between("{", "}") != ""){
		temp_pack.init(fndr.get_buffer());
		branch2.insert(map<string, package>::value_type(temp_pack.sum(), temp_pack));
	}
	
	
	
	auto it1 = branch1.begin();
	auto it2 = branch2.begin();
// branch1 - branch2 = vec1
	while(it1 != branch1.end() && it2 != branch2.end()){
		if(it1->first < it2->first){
			vec1.push_back(it1->second);
			++it1;
		}
		else if(it1->first == it2->first){
			++it1;
			++it2;
		}
		else{
			++it2;
		}
	}
	while(it1 != branch1.end()){
		vec1.push_back(it1->second);
		++it1;
	}
	cout << vec1.size() << " unique packages found in branch " << branch1_name << endl;
	
	
	
	it1 = branch1.begin();
	it2 = branch2.begin();
// branch2 - branch1 = vec2
	while(it1 != branch1.end() && it2 != branch2.end()){
		if(it1->first > it2->first){
			vec2.push_back(it2->second);
			++it2;
		}
		else if(it1->first == it2->first){
			++it1;
			++it2;
		}
		else{
			++it1;
		}
	}
	while(it2 != branch2.end()){
		vec2.push_back(it2->second);
		++it2;
	}
	cout << vec2.size() << " unique packages found in branch " << branch2_name << endl;
	
	
	
// vec3	
	it2 = branch2.begin();
	long max_ver_branch2 = 0, temp_ver = 0;
	string ver = "";
	while(it2 != branch2.end()){
		temp_ver = string_to_ver(it2->second.version);
		if(temp_ver > max_ver_branch2){
			max_ver_branch2 = temp_ver;
			ver = it2->second.version;
		}
		++it2;
	}
	cout << "Maximum version in branch " << branch2_name << " = " << ver << endl;
	
	it1 = branch1.begin();
	while(it1 != branch1.end()){
		temp_ver = string_to_ver(it1->second.version);
		if(temp_ver > max_ver_branch2){
			vec3.push_back(it1->second);
		}
		++it1;
	}
	cout << "In branch " << branch1_name << " found " << vec3.size() << " packages with a larger version" << endl;



// to file
	string file_name = "output.json";
	ofstream out;
	out.open(file_name);
	if(out.is_open()){
		out << "{ ";
		out << "\"unique_branch1_length\": " << to_string(vec1.size()) << ", ";
		out << "\"unique_branch1\": [ ";
		for(int i = 0; i < vec1.size(); ++i){
			out << vec1[i].get_json();
			if(i != vec1.size() - 1){
				out << ", ";
			}
		}
		out << "], ";
		
		out << "\"unique_branch2_length\": " << to_string(vec2.size()) << ", ";
		out << "\"unique_branch2\": [ ";
		for(int i = 0; i < vec2.size(); ++i){
			out << vec2[i].get_json();
			if(i != vec2.size() - 1){
				out << ", ";
			}
		}
		out << "], ";
		
		out << "\"max_versions_length\": " << to_string(vec3.size()) << ", ";
		out << "\"max_versions\": [ ";
		for(int i = 0; i < vec3.size(); ++i){
			out << vec3[i].get_json();
			if(i != vec3.size() - 1){
				out << ", ";
			}
		}
		out << "]";
		out << " }";
		out.close();
		cout << "Data saved in file: " << file_name << endl;
	}else{
		cout << "ERROR OPEN FILE: " << file_name << endl;
	}

	return 0;
}



long string_to_ver(string str)
{
	// really? version 2017060201, how does it compare?
	const int MAX_COUNTER = 4;
	long arr[MAX_COUNTER] = {0, 0, 0, 0};
	int counter = 0;
	long mul = 10000;
	string buff = "";
	for(int i = 0; i < str.length() + 1 && counter < MAX_COUNTER; ++i){
		if(str[i] == '.' || str[i] == 0){
			arr[counter] = num_or_bullshit(buff);
			++counter;
			buff = "";
		}else{
			buff += str[i];
		}
	}
	return arr[0] * mul * mul * mul + arr[1] * mul * mul + arr[2] * mul + arr[3];
}

int num_or_bullshit(string str)
{
	for(int i = 0; i < str.length(); ++i){
		if(str[i] < '0' || str[i] > '9'){
			return 0;
		}
	}
	return stoi(str);
}
