void kernel_main() {
  char* VIDEO_MEM = (char*) 0xb8000;
  VIDEO_MEM[0] = 'X';
  VIDEO_MEM[1] = 0x0f;
}
