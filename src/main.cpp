#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <chrono>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

std :: multimap<std :: string, unsigned long> processMap;

void killProcessesByName(std :: vector<std :: string> tasks);

int main() {
  clock_t main_start_time { clock() };

  std :: vector<std :: string> tasksk;
  tasksk.push_back(std :: string("a.exe"));
  tasksk.push_back(std :: string("b.exe"));
  tasksk.push_back(std :: string("c.exe"));
  tasksk.push_back(std :: string("d.exe"));
  tasksk.push_back(std :: string("e.exe"));
  tasksk.push_back(std :: string("f.exe"));
  tasksk.push_back(std :: string("g.exe"));
  tasksk.push_back(std :: string("h.exe"));

  killProcessesByName(tasksk);
  
  std :: cout << "Total program time: " << float (clock() - main_start_time) << "\n";
  
  // Sort array alphabetically
  // std :: sort(tasksk.begin(), tasksk.end(), sortAlphabetically);
  
  return 0;
}


void killProcessesByName(std :: vector<std :: string> tasks) {
  clock_t begin_time { clock() };

  HANDLE processes_snapshot { CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
  PROCESSENTRY32 process {};
  process.dwSize = sizeof(PROCESSENTRY32);
  
  BOOL processes_remaining = Process32First(processes_snapshot, &process);

  if (!processes_remaining) {
    return;
  }

  processMap.clear();
  
  while (processes_remaining) {
    processMap.insert({process.szExeFile, process.th32ProcessID});

    processes_remaining = Process32Next(processes_snapshot, &process);
  }

  std :: cout << "Time for getting processes: " << float (clock() - begin_time) << "\n";

  // for (const auto &elem: processMap) {
  //   std :: cout << elem.first << " -> " << elem.second << "\n";
  // }

  begin_time = clock();

  for (const auto &task: tasks) {
    auto pids = processMap.equal_range(task);
    for (auto it = pids.first; it != pids.second; it ++) {
      const auto kprocess = OpenProcess(PROCESS_TERMINATE, false, it -> second);
      if (kprocess) {
        TerminateProcess(kprocess, 1);
        CloseHandle(kprocess);
      }
    }
  }

  std :: cout << "Time for killing processes: " << float (clock() - begin_time) << "\n";
  
  CloseHandle(processes_snapshot);
}

