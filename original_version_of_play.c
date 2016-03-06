#include <ao/ao.h>
#include <mpg123.h>
#include <stdio.h>
#include <unistd.h>

#define BITS 8

int main(int argc, char *argv[]){
	mpg123_handle *mh;
	unsigned char *buffer;
	size_t buffer_size;
	size_t done;
	int err;

	int driver;
	ao_device *dev;

	ao_sample_format format;
	int channels, encoding;
	long rate;

	if(argc < 2)
		exit(0);

	unsigned int buffer_offset;
	unsigned long int buffer_stop;

	/* initializations */
	ao_initialize();
	driver = ao_default_driver_id();
	mpg123_init();
	mh = mpg123_new(NULL, &err);
//	buffer_size = mpg123_outblock(mh);
	buffer_size = 16/8*2*48000; // 192000
	buffer_offset = 4;
	buffer_stop = buffer_size / ((unsigned long int)buffer_offset);
	buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

	/* open the file and get the decoding format */
	mpg123_open(mh, argv[1]);
	if(mh == NULL)
		exit(0);
	mpg123_getformat(mh, &rate, &channels, &encoding);
	printf(" *** rate = %ld *** \n", rate);
	printf(" *** channels = %d *** \n", channels);
	printf(" *** encoding = %d *** \n", encoding);
	printf(" *** buffer_size = %d *** \n", buffer_size);

	if(encoding == MPG123_ENC_8)
		printf(" *** encoding == MPG123_ENC_8 *** \n");
	if(encoding == MPG123_ENC_16)
		printf(" *** encoding == MPG123_ENC_16 *** \n");
	if(encoding == MPG123_ENC_24)
		printf(" *** encoding == MPG123_ENC_24 *** \n");
	if(encoding == MPG123_ENC_32)
		printf(" *** encoding == MPG123_ENC_32 *** \n");
	if(encoding == MPG123_ENC_SIGNED)
		printf(" *** encoding == MPG123_ENC_SIGNED *** \n");
	if(encoding == MPG123_ENC_FLOAT)
		printf(" *** encoding == MPG123_ENC_FLOAT *** \n");
	if(encoding == MPG123_ENC_FLOAT)
		printf(" *** encoding == MPG123_ENC_FLOAT *** \n");
	if(encoding == MPG123_ENC_SIGNED_32)
		printf(" *** encoding == MPG123_ENC_SIGNED_32 *** \n");
	if(encoding == MPG123_ENC_SIGNED_24)
		printf(" *** encoding == MPG123_ENC_SIGNED_24 *** \n");
	if(encoding == MPG123_ENC_SIGNED_16)
		printf(" *** encoding == MPG123_ENC_SIGNED_16 *** \n");
	if(encoding == MPG123_ENC_SIGNED_8)
		printf(" *** encoding == MPG123_ENC_SIGNED_8 *** \n");
	if(encoding == MPG123_ENC_UNSIGNED_32)
		printf(" *** encoding == MPG123_ENC_UNSIGNED_32 *** \n");
	if(encoding == MPG123_ENC_UNSIGNED_24)
		printf(" *** encoding == MPG123_ENC_UNSIGNED_24 *** \n");
	if(encoding == MPG123_ENC_UNSIGNED_16)
		printf(" *** encoding == MPG123_ENC_UNSIGNED_16 *** \n");
	if(encoding == MPG123_ENC_UNSIGNED_8)
		printf(" *** encoding == MPG123_ENC_UNSIGNED_8 *** \n");
	if(encoding == MPG123_ENC_ULAW_8)
		printf(" *** encoding == MPG123_ENC_ULAW_8 *** \n");
	if(encoding == MPG123_ENC_ALAW_8)
		printf(" *** encoding == MPG123_ENC_ALAW_8 *** \n");
	if(encoding == MPG123_ENC_FLOAT_32)
		printf(" *** encoding == MPG123_ENC_FLOAT_32 *** \n");
	if(encoding == MPG123_ENC_FLOAT_64)
		printf(" *** encoding == MPG123_ENC_FLOAT_64 *** \n");
	if(encoding == MPG123_ENC_ANY)
		printf(" *** encoding == MPG123_ENC_ANY *** \n");

/*
mpg123_enc_enum {
	MPG123_ENC_8 = 0x00f,
	MPG123_ENC_16 = 0x040,
	MPG123_ENC_24 = 0x4000,
	MPG123_ENC_32 = 0x100,
	MPG123_ENC_SIGNED = 0x080,
	MPG123_ENC_FLOAT = 0xe00,
	MPG123_ENC_SIGNED_16 = (MPG123_ENC_16|MPG123_ENC_SIGNED|0x10),
	MPG123_ENC_UNSIGNED_16 = (MPG123_ENC_16|0x20),
	MPG123_ENC_UNSIGNED_8 = 0x01,
	MPG123_ENC_SIGNED_8 = (MPG123_ENC_SIGNED|0x02),
	MPG123_ENC_ULAW_8 = 0x04,
	MPG123_ENC_ALAW_8 = 0x08,
	MPG123_ENC_SIGNED_32 = MPG123_ENC_32|MPG123_ENC_SIGNED|0x1000,
	MPG123_ENC_UNSIGNED_32 = MPG123_ENC_32|0x2000,
	MPG123_ENC_SIGNED_24 = MPG123_ENC_24|MPG123_ENC_SIGNED|0x1000,
	MPG123_ENC_UNSIGNED_24 = MPG123_ENC_24|0x2000,
	MPG123_ENC_FLOAT_32 = 0x200,
	MPG123_ENC_FLOAT_64 = 0x400,
	MPG123_ENC_ANY
}
*/




	/* set the output format and open the output device */
	format.bits = mpg123_encsize(encoding) * BITS;
	format.rate = rate;
	format.channels = channels;
	format.byte_format = AO_FMT_NATIVE;
	format.matrix = 0;
	dev = ao_open_live(driver, &format, NULL);

	signed short int *buffer_one, *buffer_two;
	printf(" *** format.bits = %d *** \n", format.bits);
	/* decode and play */
	unsigned int iInc;
	while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
//		ao_play(dev, buffer, done);
		for(iInc = 0; iInc < buffer_stop; iInc++){
			buffer_one = (signed short int*)(buffer+((size_t)(iInc*buffer_offset)));
			buffer_two = (signed short int*)(buffer+((size_t)(iInc*buffer_offset+buffer_offset/2)));
			fprintf(stdout,"%#04x %#04x %#04x %#04x		%5hd %5hd\n",
				(buffer+iInc*buffer_offset)[0],
				(buffer+iInc*buffer_offset)[1],
				(buffer+iInc*buffer_offset)[2],
				(buffer+iInc*buffer_offset)[3],
				*buffer_one, *buffer_two
			);
		}
		printf(" *** done = %d *** \n", done);
			fprintf(stdout,"\n");
//		break;	
	}
	fclose(stdout);
	/* clean up */
	free(buffer);
	ao_close(dev);
	mpg123_close(mh);
	mpg123_delete(mh);
	mpg123_exit();
	ao_shutdown();

	return 0;
}
