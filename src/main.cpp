#include <iostream>
#include <fstream>
#include <algorithm>

#include <vector>
#include <stack>
#include <string>
#include <map>

#include <chrono>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>


std :: multimap<std :: string, unsigned long> processMap;

std :: multimap<unsigned long, unsigned long> parentidtoid;

void refreshProcesses(void);

void killProcessesByName(std :: vector<std :: string> tasks);

void killProcessesByID(std :: vector<unsigned long> tasks);

std :: vector<std :: string> getTasksForKey(char key);

std :: vector<unsigned long> findChildren(unsigned long pid);

// int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd) {
int main() {
  // AllocConsole();
  // freopen("CONOUT$", "w", stdout); // what do these do?

  // clock_t main_start_time { clock() };

  std :: vector<std :: string> tasksk = getTasksForKey('k');
  // std :: vector<std :: string> tasksj = getTasksForKey('j');
  // std :: vector<std :: string> tasksl = getTasksForKey('l');
  
  /*
  for (const auto &elem: tasksk) {
    std :: cout << elem << "\n";
  }
  */

  // killProcessesByName(tasksk);
  
  int pid {};
  std :: cout << "PID to get children of: ";
  std :: cin >> pid;

  refreshProcesses();

  std :: vector<unsigned long> childrenpids { findChildren(pid) };

  for (const auto& id: childrenpids) {
    std :: cout << id << " ";
  }

  std :: reverse(std :: begin(childrenpids), std :: end(childrenpids));

  killProcessesByID(childrenpids);

  // std :: vector<std :: string> taskstokill = {"Taskmgr.exe"};
  // killProcessesByName(taskstokill);

  return 0;

  /*

  RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT, 'K');
  RegisterHotKey(NULL, 2, MOD_CONTROL | MOD_ALT, 'J');
  RegisterHotKey(NULL, 3, MOD_CONTROL | MOD_ALT, 'L');
  RegisterHotKey(NULL, 4, MOD_CONTROL | MOD_ALT, 'Q');

  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0)) {
    if (msg.message == WM_HOTKEY) {
      switch (msg.wParam) {
        case 1:
          std :: cout << "ctrl alt k pressed\n";
          killProcessesByName(tasksk);
          break;
        case 2:
          killProcessesByName(tasksj);
          break;
        case 3:
          killProcessesByName(tasksl);
          break;
        case 4:
          return 0;
          break;
      }
    }

    else {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  // Handles any other messages (harmless if no windows)
    } // REMOVE THIS ELSE AFTER UNCOMMENTING MAYBE? MIGHT BE USEFUL THOUGH, FIND OUT WHAT IT DOES
  }
  */
}

void refreshProcesses() {
  HANDLE processes_snapshot { CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
  PROCESSENTRY32 process {};
  process.dwSize = sizeof(PROCESSENTRY32);
  
  BOOL processes_remaining = Process32First(processes_snapshot, &process);

  if (!processes_remaining) {
    return;
  }

  processMap.clear();

  parentidtoid.clear();
  
  while (processes_remaining) {
    processMap.insert({process.szExeFile, process.th32ProcessID});

    parentidtoid.insert({process.th32ParentProcessID, process.th32ProcessID});

    processes_remaining = Process32Next(processes_snapshot, &process);
  }

  CloseHandle(processes_snapshot);
}

void killProcessesByName(std :: vector<std :: string> tasks) {
  for (const auto &task: tasks) {
    auto pids = processMap.equal_range(task);
    for (auto it = pids.first; it != pids.second; it ++) {
      const auto process_handle = OpenProcess(PROCESS_TERMINATE, false, it -> second);
      if (process_handle) {
        TerminateProcess(process_handle, 1);
      }
      CloseHandle(process_handle);
    }
  }
}

void killProcessesByID(std :: vector<unsigned long> tasks) {
  for (const auto& task: tasks) {
    const auto process_handle = OpenProcess(PROCESS_TERMINATE, false, task);
    if (process_handle) {
      TerminateProcess(process_handle, 1);
    }
    CloseHandle(process_handle);
  }
}

std :: vector<std :: string> getTasksForKey(char key) {
  std :: string filename = "tasks";
  filename += key;
  filename += ".txt";
  std :: ifstream f(filename);
  std :: vector<std :: string> tasks;
  std :: string task;

  while (std :: getline(f, task)) {
    if (task.empty()) {
      continue;
    }
    if (task.back() == '\r') {
      task.pop_back();
    }
    tasks.push_back(task);
  }

  return tasks;
}

std :: vector<unsigned long> findChildren(unsigned long pid) {
  std :: vector<unsigned long> pidlist;
  pidlist.push_back(pid);
  std :: vector<unsigned long> temp;
  auto childrenids = parentidtoid.equal_range(pid);
  for (auto it = childrenids.first; it != childrenids.second; it ++) {
    // pidlist.push_back(it -> second);
    temp = findChildren(it -> second);
    pidlist.insert(pidlist.end(), temp.begin(), temp.end());
  }

  return pidlist;
}



