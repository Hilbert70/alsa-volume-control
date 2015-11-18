
#include <alsa/asoundlib.h>
//#include <alsa/control.h>
#include <alsa/mixer.h>

// #include "asla.h"

/* start ALSA stuff ---------------------------- */
static const char alsa_core_devnames[] = "default";
static char *card, *channel;
static int muted = 0;
static int mutecount = 0;
static snd_mixer_t *handle = NULL;
static snd_mixer_elem_t *elem = NULL;

static long alsa_min, alsa_max, alsa_vol;

/* Volume saved to file */
static int alsa_get_unmute_volume( void )
{
    long val;
    assert (elem);

    if (snd_mixer_selem_is_playback_mono(elem)) {
        snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_MONO, &val);
        return val;
    } else {
        int c, n = 0;
        long sum = 0;
        for (c = 0; c <= SND_MIXER_SCHN_LAST; c++) {
            if (snd_mixer_selem_has_playback_channel(elem, c)) {
                snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &val);
                sum += val;
                n++;
            }
        }
        if (! n) {
            return 0;
        }

        val = sum / n;
        sum = (long)((double)(alsa_vol * (alsa_max - alsa_min)) / 100. + 0.5);

        if (sum != val) {
           alsa_vol = (long)(((val * 100.) / (alsa_max - alsa_min)) + 0.5);
        }
        return alsa_vol;
    }
}

static int alsa_get_volume( void )
{
    if (muted)
        return 0;
    else
        return alsa_get_unmute_volume();
}

static int alsa_set_volume( int percentdiff )
{
    long volume;

    alsa_get_volume();

    alsa_vol += percentdiff;
    if( alsa_vol > 100 ) alsa_vol = 100;
    if( alsa_vol < 0 ) alsa_vol = 0;

    volume = (long)((alsa_vol * (alsa_max - alsa_min) / 100.) + 0.5);

    snd_mixer_selem_set_playback_volume_all(elem, volume + alsa_min);
    snd_mixer_selem_set_playback_switch_all(elem, 1);
    muted = 0;
    mutecount = 0;

    return alsa_vol;
}

static void alsa_mute( int mute )
{
    if( !mute && mutecount ) mutecount--;
    if( mutecount ) return;

    if( mute ) {
        mutecount++;
        muted = 1;
        if (snd_mixer_selem_has_playback_switch(elem))
            snd_mixer_selem_set_playback_switch_all(elem, 0);
        else
            fprintf(stderr, "mixer: mute not implemented\n");
    } else {
        muted = 0;
        if (snd_mixer_selem_has_playback_switch(elem))
            snd_mixer_selem_set_playback_switch_all(elem, 1);
        else
            fprintf(stderr, "mixer: mute not implemented\n");
    }
}

static int alsa_ismute( void )
{
    return muted;
}

void SetAlsaMasterVolume(long volume)
{
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Master";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

    snd_mixer_close(handle);
}
/* end ALSA stuff ---------------------------- */

void SetAlsaMasterMute()
{
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Master";
    int   state;

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    if (snd_mixer_selem_has_playback_switch(elem)) {
	snd_mixer_selem_get_playback_switch(elem,0,&state);
	fprintf(stderr,"State %d\n",state);
	
        snd_mixer_selem_set_playback_switch_all(elem, !state);
    }

    snd_mixer_close(handle);
}
