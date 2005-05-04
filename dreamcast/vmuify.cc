#include <kos.h>
extern unsigned char icon_data[];

void vmuify(char *filename_in, char *filename_out, char *filename) {
  int fd,pkg_size;
  uint8 *buf;
  uint8 *pkg_out;
  vmu_pkg_t pkg;
  
  printf("Opening source file\n");
  fd=fs_open(filename_in,O_RDONLY);
  buf=(uint8 *)malloc(fs_total(fd));
  fs_read(fd,buf,fs_total(fd));
  printf("Read %i bytes\n",fs_total(fd));
 
  strcpy(pkg.desc_short, filename);
  strcpy(pkg.desc_long, "DCSquares Settings");
  strcpy(pkg.app_id, "DCSQ");
  pkg.icon_cnt = 1;
  pkg.icon_anim_speed = 1;
  memcpy(&pkg.icon_pal[0],icon_data,32);
  pkg.icon_data=icon_data+32;
  pkg.eyecatch_type = VMUPKG_EC_NONE;
  pkg.data_len = fs_total(fd);
  pkg.data=buf;
  printf("Building package\n");
  vmu_pkg_build(&pkg, &pkg_out, &pkg_size);
  fs_close(fd);
  printf("Closing source file\n");

  printf("Opening output file\n");
  fd=fs_open(filename_out,O_WRONLY);
	if(fd==-1) {
	  printf("Error opening output file.\n"); 
		return;
	}
  printf("Writing..\n");
  fs_write(fd,pkg_out,pkg_size);
  printf("Closing output file\n");
  fs_close(fd);
  free(buf);
  printf("VMUification complete\n");
}
