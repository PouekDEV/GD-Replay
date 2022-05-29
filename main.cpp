#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include "json.hpp"
#include <fstream>
#include <chrono>
#include <vector>
#include <numeric>
#include <iomanip>
#include <thread>
#include <algorithm>
#pragma comment(lib, "Winmm.lib")
using namespace std;
using json = nlohmann::json;

// Values
void inputsimulate();
void startrecording();
void startreplay();
void remove(vector<int> &v);
string choose;
string filename;
int main(){
	//MessageBox(NULL, "Error occured", "GD:Replay", MB_ICONERROR | MB_OK);
	//system("cls");
	cout << "\n ===================== \n GD:Replay \n (Doesn't work on the ship parts :)) \n =====================" << endl;
	cout << "\n 1 - start recording \n 2 - replay from file \n 3 - exit" << endl;
	cin >> choose;
	if(choose == "1"){
		startrecording();	
	}
	else if(choose == "2"){
		startreplay();
	}
	else if(choose == "3"){
		exit(0);
	}
	else{
		main();
	}
}
void startreplay(){
	bool startreplay = false;
	bool replay_finished = false;
	int nextone = 1;
	system("cls");
	cout << "\n Input filename (must be in the same directory as the program)" << endl;
	cin >> filename;
	ifstream i(filename + ".json");
	if(i.fail()){
        MessageBox(NULL, "File not found", "GD:Replay", MB_ICONERROR | MB_OK);
        main();
    }
	json j;
	i >> j;
	vector<int> inputs;
	inputs = j["inputs"].get<vector<int>>();
	int lastone = inputs.size();
	system("cls");
	cout << "\n Press F9 to start replay \n Press F10 to stop" << endl;
	while(!startreplay){
		if(GetKeyState(VK_F9) & 0x8000){
			system("cls");
			cout << "Playing replay " << filename << " now" << endl;
			startreplay = true;
		}
	}
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	while(startreplay && !replay_finished){
		if(GetKeyState(VK_F10) & 0x8000){
			main();
			break;
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		float currenttime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		cout << inputs[nextone] << " ms " << nextone << " is the current click. Current time " << currenttime << endl;
		if(currenttime == inputs[nextone]){
			inputsimulate();
			nextone += 1;
		}
		else if(currenttime > inputs[nextone]){
			inputsimulate();
			nextone += 1;
		}
		if(nextone >= lastone){
			cout << "Finished replay" << endl;
			main();
			break;
		}
	}
}
void startrecording(){
	bool record = false;
	bool stopped_recording = false;
	vector<int> inputs;
	system("cls");
	cout << "\n Input a name for the file" << endl;
	cin >> filename;
	system("cls");
	cout << "\n Press F9 to start recording \n Press F10 to stop recording" << endl;
	while(!record && !stopped_recording){
		if(GetKeyState(VK_F9) & 0x8000){
			system("cls");
			cout << "Now recording " << filename << " replay" << endl;
			record = true;
		}
	}
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	inputs.push_back(0);
	while(record){
		if(GetKeyState(VK_LBUTTON) < 0){
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			float stamp = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			cout << "Clicked at " << stamp << " ms" << endl;
			inputs.push_back(stamp);
		}
		if(GetKeyState(VK_F10) & 0x8000){
			remove(inputs);
			json j = {
				{"inputs",inputs}
			};
			ofstream o(filename + ".json");
			o << setw(4) << j << endl;
			stopped_recording = true;
			record = false;
			main();
		}
	}
}
void inputsimulate()
{	
	INPUT space;
	space.type = INPUT_KEYBOARD;
	space.ki.time = 0;
	space.ki.wVk = 0;
	space.ki.dwFlags = KEYEVENTF_SCANCODE;
	space.ki.wScan = 0x39;
	SendInput(1, &space, sizeof(INPUT));
	space.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &space, sizeof(INPUT));
}
// https://www.techiedelight.com/remove-duplicates-vector-cpp/
void remove(vector<int> &v)
{
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }
    v.erase(end, v.end());
}
