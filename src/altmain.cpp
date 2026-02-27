#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include <chrono>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

std :: multimap<std :: string, unsigned long> processMap;

void killProcessesByName(std :: vector<std :: string> tasks);

std :: vector<std :: string> getTasksForKey(char key);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd) {
  // AllocConsole();
  // freopen("CONOUT$", "w", stdout); // what do these do?

  // clock_t main_start_time { clock() };

  std :: vector<std :: string> tasksk = getTasksForKey('k');
  std :: vector<std :: string> tasksj = getTasksForKey('j');
  std :: vector<std :: string> tasksl = getTasksForKey('l');
  
  /*
  for (const auto &elem: tasksk) {
    std :: cout << elem << "\n";
  }
  */

  RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT, 'K');
  RegisterHotKey(NULL, 2, MOD_CONTROL | MOD_ALT, 'J');
  RegisterHotKey(NULL, 3, MOD_CONTROL | MOD_ALT, 'L');
  RegisterHotKey(NULL, 4, MOD_CONTROL | MOD_ALT, 'Q');

  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0)) {
    if (msg.message == WM_HOTKEY) {
      switch (msg.wParam) {
        case 1:
          // std :: cout << "ctrl alt k pressed\n";
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

    /*
    else {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  // Handles any other messages (harmless if no windows)
    }
    */
  }
}


void killProcessesByName(std :: vector<std :: string> tasks) {
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

  CloseHandle(processes_snapshot);
}

std :: vector<std :: string> getTasksForKey(char key) {
  std :: string filename = "D:/dev/cpp/tasks/tasks";
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

