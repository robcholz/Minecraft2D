#import <Foundation/Foundation.h>
#include <libproc.h>
#include <sys/utsname.h>
#include "resource/System.hpp"

void OS::init() {
  NSOperatingSystemVersion version =
      [[NSProcessInfo processInfo] operatingSystemVersion];
  NSProcessInfo* info = [NSProcessInfo processInfo];
  struct utsname uts {};
  uname(&uts);
  processor_count = std::thread::hardware_concurrency();
  os_name = "macOS";
  os_version = std::to_string(version.majorVersion) + "." +
               std::to_string(version.minorVersion) + "." +
               std::to_string(version.patchVersion);
  os_architecture = uts.machine;
  cpu_model_name =
      utils::execCommand("sysctl -n machdep.cpu.brand_string").first;
  gpu_model_name = getExecCommandString(
      "system_profiler SPDisplaysDataType | grep Chipset", "Chipset Model:");
  screen_info = getExecCommandString(
      "system_profiler SPDisplaysDataType | grep Resolution", "Resolution:");
  pid = getpid();
  RAM_total = [info physicalMemory] / 1024 / 1024;
}

uint64_t OS::getProcessMemoryUsage() {
  struct proc_taskinfo taskInfo {};
  int ret =
      proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &taskInfo, PROC_PIDTASKINFO_SIZE);
  if (ret <= 0) {
    return 0;
  }
  return taskInfo.pti_resident_size / 1024 / 1024;
}

uint8_t OS::getProcessCPUUsagePercent() {
  struct proc_taskallinfo taskAllInfo {};
  int ret = proc_pidinfo(pid, PROC_PIDTASKALLINFO, 0, &taskAllInfo,
                         PROC_PIDTASKALLINFO_SIZE);
  if (ret <= 0) {
    return 0.0;
  }
  unsigned long totalSystemTime = taskAllInfo.ptinfo.pti_total_system;
  unsigned long totalUserTime = taskAllInfo.ptinfo.pti_total_user;
  unsigned long totalCPUTime = totalSystemTime + totalUserTime;
  unsigned long taskCPUTime =
      taskAllInfo.ptinfo.pti_total_user + taskAllInfo.ptinfo.pti_total_system;
  double cpuUsagePercent = ((double)taskCPUTime / (double)totalCPUTime) * 100.0;
  return (uint8_t)cpuUsagePercent;
}
