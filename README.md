# GLKIo2-Research
GLKIo2.sys Research (PoC)
# Usage
- sc.exe create GLKIo2 binpath=[Path to]\GLKIO2.sys type=kernal
- sc.exe start GLKIo2
  - Run compiled main.cpp from command line cmd.exe (Non-admin)
# In the function sub_140001B20, the most relevant IOCTL codes being handled are:
- 0x80102040 (case -2146426816)
- 0x80102044 (case -2146426812)
 - 0x80102050 (case -2146426800, which matches the IOCTL_GLCKIO_VMWRITE being used)
- 0x80102054 (case -2146426796)
- 0x80102058 (case -2146426792)
- 0x8010205C (case -2146426788)
- 0x80102060 (case -2146426784)
