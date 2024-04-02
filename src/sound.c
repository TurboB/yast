/*  2024-03-29 17:00  */
/*
    yast - yet another slotcar timer
	File: sound.c -> contains some sound routines of the project

    Copyright (C)  2016 - 2024 Martin Berentsen


    This file is part of yast.

    yast is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    yast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with yast.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <math.h>

#ifdef ALSA_SOUND
	#include <alsa/asoundlib.h>
	#include <alsa/pcm.h>
#endif /* ALSA_SOUND */

#include "sound.h"
#include "yast_define.h"
#include "config.h"

extern short snd_buffer[SND_NUMBER_OF_TONES] [SND_BUFFER_SIZE];	/* define default the tone arrays */
extern unsigned long snd_buffer_len[SND_NUMBER_OF_TONES];  		/* set length of the sounds inside the buffer */
extern struct CONFIG config;									/* announce the config memory*/

#ifdef ALSA_SOUND
snd_pcm_t *playback_handle;
snd_pcm_sframes_t frames;
#endif /* ALSA_SOUND */

/************************************************************************
SND_generate defaults(buffer_len,sample_rate)
There are only sine sound used with some different frequencies and 
some different envelopes. For me this is O.K.
************************************************************************/
int SND_generate_defaults(int srate)
{

    int freq;        		/* frequency */
    int k;			/* dummy */
    int buffer_len;
    
    /* example: 48000 * 0,16 = 7680, * 0,2 = 9600 */		

    /* printf("Sound Buffer MAX: %d \n",SND_BUFFER_SIZE); */

    /* default SINE WAVE Tone generatoion */
    
	/* tone 0  ( power off ) */
	freq = 300;                	/* frequency */
	buffer_len = 11024;		/* < SND_BUFFER_SIZE */

	if(buffer_len > SND_BUFFER_SIZE) {
		fprintf(stderr,"SND_generator: buffer size smaller than sound\n");
		exit(EXIT_FAILURE);
	}

    for (k=0; k<(buffer_len/2); k++){
       snd_buffer[0][k] = (short) ( SND_S16_CORRECTION * (sin(2*M_PI*freq/srate*k)) );                 /* sine wave value generation */
    }

    for (k=(buffer_len/2); k<(buffer_len); k++){
       snd_buffer[0][k] = (short) ( SND_S16_CORRECTION * (sin(2*M_PI*freq*1.75/srate*k)) );                 /* sine wave value generation */
    }       
    snd_buffer_len[0] = buffer_len;

	/* tone 1  ( power on ) */
	freq = 300;                	/* frequency */
	buffer_len = 11024;		/* < SND_BUFFER_SIZE */

	if(buffer_len > SND_BUFFER_SIZE) {
		fprintf(stderr,"SND_generator: buffer size smaller than sound\n");
		exit(EXIT_FAILURE);
	}

    for (k=0; k<(buffer_len/2); k++){
        snd_buffer[1][k] = (short) ( SND_S16_CORRECTION * (sin(2*M_PI*1.75*freq/srate*k)) );                 /* sine wave value generation */
    }       
    for (k=(buffer_len/2); k<(buffer_len); k++){
        snd_buffer[1][k] = (short) ( SND_S16_CORRECTION * (sin(2*M_PI*freq/srate*k))  );                 /* sine wave value generation */
    }       
    snd_buffer_len[1] = buffer_len;

	/* tone 2  ( valid timing ) */
	freq = 960;			/* frequency */
	buffer_len = 8820;		/* < SND_BUFFER_SIZE */
	
	if(buffer_len > SND_BUFFER_SIZE) {
		fprintf(stderr,"SND_generator: buffer size smaller than sound\n");
		exit(EXIT_FAILURE);
	}

    for (k=0; k<(buffer_len); k++){
       snd_buffer[2][k] = (short) ( SND_S16_CORRECTION * (sin(2*M_PI*freq/srate*k)) * sin((float)k/(float)buffer_len*(float)M_PI));                 /* sine wave value generation */
    }       
    snd_buffer_len[2] = buffer_len;

	/* tone 3  ( non valid timing ) */
	freq = 590;			/* frequency */
	buffer_len = 8820;		/* < SND_BUFFER_SIZE */
	
	if(buffer_len > SND_BUFFER_SIZE) {
		fprintf(stderr,"SND_generator: buffer size smaller than sound\n");
		exit(EXIT_FAILURE);
	}

    for (k=0; k<(buffer_len); k++){
       snd_buffer[3][k] = (short) ( SND_S16_CORRECTION * (sin(2*M_PI*freq/srate*k)) * sin((float)k/(float)buffer_len*(float)M_PI));                 /* sine wave value generation */
    }       
    snd_buffer_len[3] = buffer_len;

	/* tone 4  ( timing end after time race ) */
	freq = 175;			/* frequency */
	buffer_len = 16000;		/* <= SND_BUFFER_SIZE */
	
	if(buffer_len > SND_BUFFER_SIZE) {
		fprintf(stderr,"SND_generator: buffer size smaller than sound\n");
		exit(EXIT_FAILURE);
	}

    for (k=0; k<(buffer_len); k++){
       snd_buffer[4][k] = (short) ( SND_S16_CORRECTION * (sin(2*M_PI*freq/srate*k)) );                 /* sine wave value generation */
    }       
    snd_buffer_len[4] = buffer_len;

	printf("- ALSA SETUP with default %d different sounds\n",SND_NUMBER_OF_TONES);

    return 0;
}

/************************************************************************
SND_setup()
************************************************************************/

int SND_setup(void)
{
#ifdef ALSA_SOUND

    int err;
	int dir;

    static char *device = "default";                       /* soundcard */
    snd_pcm_hw_params_t *hw_params;

    snd_pcm_uframes_t buffer_size;
    snd_pcm_uframes_t period_size;
      
    if ((err = snd_pcm_open(&playback_handle, device, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0) {
            fprintf(stderr,"Playback open error: %s\n", snd_strerror(err));
        exit(0);
    }


	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
				 snd_strerror (err));
		exit (0);
	}

	if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
				snd_strerror (err));
		exit (0);
	}
	if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n",
				 snd_strerror (err));
		exit (0);
	}
	
	if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16 )) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n",
				 snd_strerror (err));
		exit (0);
	}

 	if ((err = snd_pcm_hw_params_set_rate (playback_handle, hw_params, SND_SAMPLE_RATE , 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n",
			    snd_strerror (err));
		exit (0);
	}
	
	if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, 1)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n",
				 snd_strerror (err));
		exit (0);
	}

	if ((err = snd_pcm_hw_params_set_rate_resample(playback_handle, hw_params, 0)) < 0) {
		fprintf (stderr, "cannot set resample to zero (%s)\n",
				 snd_strerror (err));
		exit (0);
	}
	
	/* just set something bigger than used buffer size */ 
	period_size = SND_PERIOD_SIZE; 		/* 8192 */ 	
	buffer_size = SND_BUFFER_SIZE;		/* period * 2 */

	if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n",
				 snd_strerror (err));
		exit (0);
	}

   if((err = snd_pcm_hw_params_set_period_size_near(playback_handle, hw_params, &period_size, &dir)) < 0)
       {
       fprintf(stderr, "snd_pcm_hw_params_set_period_size failed");
      	exit(0);
       }
 
     if((err = snd_pcm_hw_params_set_buffer_size_near(playback_handle, hw_params, &buffer_size)) < 0)
       {
       fprintf(stderr,"snd_pcm_hw_params_set_buffer_size failed");
      	exit(0);
       }
    
    /* set hardware parameter */
	if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n",
				 snd_strerror (err));
		exit (0);
	}
	
	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (playback_handle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		exit(0);
	}

	printf("- ALSA SETUP with %d sample rate and %d buffer size\n",SND_SAMPLE_RATE,SND_BUFFER_SIZE);
	SND_generate_defaults(SND_SAMPLE_RATE);

#endif /* ALSA_SOUND */
	
    return 0;

}
/************************************************************************
SND_play(int soundnumber)
	soundnumber must be one of the designed sounds
	1: Trackpower off
	2: Trackpower on
	3: Track Pulse valid
	4: Track Pulse not valid 
	5: Timing off after time race
************************************************************************/
int SND_play(int soundnumber)
{
    if( (soundnumber <= 0) || (soundnumber > SND_NUMBER_OF_TONES) )
	return -1;

#ifdef ALSA_SOUND
	
    frames = snd_pcm_prepare(playback_handle);
    frames = snd_pcm_writei(playback_handle, snd_buffer[ soundnumber - 1 ], snd_buffer_len[ soundnumber-1] );    /* sending values to sound driver */

    if (frames < 0) {
        frames = snd_pcm_recover(playback_handle, frames, 0);
        }
    if (frames < 0) {
           fprintf(stderr,"snd_pcm_writei failed: %s\n", snd_strerror(frames));
        }
    
#endif /* ALSA_SOUND */

	return 0;
}

/************************************************************************
SND_close()
************************************************************************/
int SND_close(void)
{
#ifdef ALSA_SOUND
	snd_pcm_close (playback_handle);
#endif /* ALSA_SOUND */
	return 0;
}

