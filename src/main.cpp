#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

struct Process {
  TCHAR name;
  DWORD id;
};

bool sortAlphabetically(std :: string a, std :: string b);

void KillProcessesByName(std :: vector<std :: string> tasks);

int main() {
  std :: vector<std :: string> tasksk;
  tasksk.push_back(std :: string("a.exe"));
  tasksk.push_back(std :: string("b.exe"));
  tasksk.push_back(std :: string("c.exe"));
  tasksk.push_back(std :: string("d.exe"));
  tasksk.push_back(std :: string("e.exe"));
  tasksk.push_back(std :: string("f.exe"));
  tasksk.push_back(std :: string("g.exe"));
  tasksk.push_back(std :: string("h.exe"));

  KillProcessesByName(tasksk);
  
  // Sort array alphabetically
  // std :: sort(tasksk.begin(), tasksk.end(), sortAlphabetically);
  
  return 0;
}


void KillProcessesByName(std :: vector<std :: string> tasks) {
  HANDLE processes_snapshot { CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
  PROCESSENTRY32 process {};
  process.dwSize = sizeof(PROCESSENTRY32);
  
  BOOL processes_remaining = Process32First(processes_snapshot, &process);

  if (!processes_remaining) {
    return;
  }

  std :: multimap<std :: string, unsigned long> processMap;

  while (processes_remaining) {
    processMap.insert({process.szExeFile, process.th32ProcessID});

    processes_remaining = Process32Next(processes_snapshot, &process);
  }

  for (const auto &elem: processMap) {
    std :: cout << elem.first << " -> " << elem.second << "\n";
  }
  std :: cout << "\n";
}

bool sortAlphabetically(std :: string a, std :: string b) {
  return a < b;
}




