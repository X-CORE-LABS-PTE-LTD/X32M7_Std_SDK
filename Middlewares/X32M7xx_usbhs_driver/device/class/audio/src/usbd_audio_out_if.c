/**
 ******************************************************************************
 * @file      usbd_audio_out_if.c
 * @version   v1.0.0
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 X-CORE LABS PTE. LTD.
 * All rights reserved.
 *
 * Usage, modification, and distribution of this software, whether in source
 * code or compiled binary format, are authorized provided that the following
 * criteria are strictly met:
 *
 * 1. Any distribution of the source code must leave this copyright header,
 * the current list of conditions, and the enclosed disclaimer intact.
 * 2. When distributing in binary format, the corresponding documentation
 * and/or materials must clearly replicate this copyright notice, the
 * conditions, and the disclaimer.
 * 3. The name of the organization or its contributors cannot be utilized to
 * promote or endorse any derivative products built upon this software
 * without obtaining prior explicit written consent.
 *
 * THIS SOFTWARE PACKAGE IS DELIVERED ON AN "AS-IS" BASIS. THE COPYRIGHT
 * HOLDERS AND AUTHORS EXPRESSLY DISCLAIM ALL WARRANTIES, WHETHER EXPLICIT
 * OR IMPLIED, WHICH INCLUDES BUT IS NOT LIMITED TO ANY IMPLIED ASSURANCES
 * OF COMMERCIAL UTILITY OR FITNESS FOR A SPECIFIC REQUIREMENT. UNDER NO
 * CIRCUMSTANCES SHALL THE CREATORS OR CONTRIBUTORS BE HELD ACCOUNTABLE FOR
 * ANY DIRECT, INDIRECT, ACCIDENTAL, SPECIAL, PUNITIVE, OR SUBSEQUENT DAMAGES
 * (SUCH AS, BUT NOT LIMITED TO, PROCUREMENT OF ALTERNATIVE GOODS, LOSS OF
 * DATA, LOSS OF REVENUE, OR OPERATIONAL INTERRUPTIONS). THIS LIMITATION OF
 * LIABILITY APPLIES REGARDLESS OF THE LEGAL THEORY STATED - WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE) - ARISING FROM
 * ANY USE OF THIS CODE, EVEN IF PREVIOUSLY ADVISED OF SUCH POTENTIAL RISKS.
 ******************************************************************************
 */

#include "usbd_audio_core.h"
#include "usbd_audio_out_if.h"
#include "bsp_wm8978.h"

static uint8_t  Init         (uint32_t  AudioFreq, uint32_t Volume, uint32_t options);
static uint8_t  DeInit       (uint32_t options);
static uint8_t  AudioCmd     (uint8_t* pbuf, uint32_t size, uint8_t cmd);
static uint8_t  VolumeCtrl    (uint8_t vol);
static uint8_t  MuteCtrl      (uint8_t cmd);
static uint8_t  PeriodicTC   (uint8_t cmd);
static uint8_t  GetState     (void);

AUDIO_FOPS_TypeDef  AUDIO_OUT_fops = 
{
    Init,
    DeInit,
    AudioCmd,
    VolumeCtrl,
    MuteCtrl,
    PeriodicTC,
    GetState
};

static uint8_t AudioState = AUDIO_STATE_INACTIVE;

/**
*\*\name    Init.
*\*\fun     Initialize and configures all required resources for audio play function.
*\*\param   AudioFreq: Startup audio frequency. 
*\*\param   Volume: Startup volume to be set.
*\*\param   options: specific options passed to low layer function.
*\*\return  AUDIO_OK if all operations succeed, AUDIO_FAIL else.
**/
static uint8_t Init(uint32_t AudioFreq, uint32_t Volume, uint32_t options)
{
    static uint32_t Initialized = 0;

    /* Check if the low layer has already been initialized */
    if (Initialized == 0)
    {
        /* Call low layer function */
        if (WM8978_Speaker_Init(Volume, AudioFreq) != 0)
        {
            AudioState = AUDIO_STATE_ERROR;
            return AUDIO_FAIL;
        }
        /* Set the Initialization flag to prevent reinitializing the interface again */
        Initialized = 1;
    }
  
    /* Update the Audio state machine */
    AudioState = AUDIO_STATE_ACTIVE;

    return AUDIO_OK;
}

/**
*\*\name    DeInit.
*\*\fun     Free all resources used by low layer and stops audio-play function.
*\*\param   options: specific options passed to low layer function.
*\*\return  AUDIO_OK if all operations succeed, AUDIO_FAIL else.
**/
static uint8_t DeInit (uint32_t options)
{
    /* Update the Audio state machine */
    AudioState = AUDIO_STATE_INACTIVE;

    return AUDIO_OK;
}

uint8_t txbuff[0xC0] = {0};
/**
*\*\name    AudioCmd.
*\*\fun     Play, Stop, Pause or Resume current file.
*\*\param   pbuf: address from which file should be played.
*\*\param   size: size of the current buffer/file.
*\*\param   cmd: command to be executed, can be AUDIO_CMD_PLAY , AUDIO_CMD_PAUSE, AUDIO_CMD_RESUME or AUDIO_CMD_STOP.
*\*\return  AUDIO_OK if all operations succeed, AUDIO_FAIL else.
**/
static uint8_t AudioCmd(uint8_t* pbuf, uint32_t size, uint8_t cmd)
{
    /* Check the current state */
    if ((AudioState == AUDIO_STATE_INACTIVE) || (AudioState == AUDIO_STATE_ERROR))
    {
        AudioState = AUDIO_STATE_ERROR;
        return AUDIO_FAIL;
    }

    switch (cmd)
    {
        /* Process the PLAY command ----------------------------*/
        case AUDIO_CMD_PLAY:
            /* If current state is Active or Stopped */
            if ((AudioState == AUDIO_STATE_ACTIVE) || (AudioState == AUDIO_STATE_STOPPED) || (AudioState == AUDIO_STATE_PLAYING))
            {
                I2S_TX_DMA_Add_SIZE_Set(pbuf, (size/2));
                
                AudioState = AUDIO_STATE_PLAYING;
                return AUDIO_OK;
            }
            /* If current state is Paused */
            else if (AudioState == AUDIO_STATE_PAUSED)
            {
                I2S_Play_Start();
                AudioState = AUDIO_STATE_PLAYING;
                return AUDIO_OK;
            } 
            else /* Not allowed command */
            {
                return AUDIO_FAIL;
            }
    
        /* Process the STOP command ----------------------------*/
        case AUDIO_CMD_STOP:
            if (AudioState != AUDIO_STATE_PLAYING)
            {
                /* Unsupported command */
                return AUDIO_FAIL;
            }
            else
            {
                I2S_Play_Stop();
                AudioState = AUDIO_STATE_STOPPED;
                return AUDIO_OK;
            }

        /* Process the PAUSE command */
        case AUDIO_CMD_PAUSE:
            if (AudioState != AUDIO_STATE_PLAYING)
            {
                /* Unsupported command */
                return AUDIO_FAIL;
            }
            else
            {
                I2S_Play_Stop();
                AudioState = AUDIO_STATE_PAUSED;
                return AUDIO_OK;
            } 
    
        /* Unsupported command ---------------------------------*/
        default:
            return AUDIO_FAIL;
    }
}

/**
*\*\name    VolumeCtrl.
*\*\fun     Set the volume level in %
*\*\param   vol: volume level to be set in % (from 0% to 100%)
*\*\return  AUDIO_OK if all operations succeed, AUDIO_FAIL else.
**/
static uint8_t VolumeCtrl(uint8_t vol)
{
    /* Call low layer volume setting function */  
    WM8978_SetOUT1Volume(vol);

    return AUDIO_OK;
}

/**
*\*\name    MuteCtrl.
*\*\fun     Mute or Unmute the audio current output
*\*\param   cmd: can be 0 to unmute, or 1 to mute.
*\*\return  AUDIO_OK if all operations succeed, AUDIO_FAIL else.
**/
static uint8_t MuteCtrl(uint8_t cmd)
{
    /* Call low layer mute setting function */  
    WM8978_OutMute(cmd);
    return AUDIO_OK;
}

/**
*\*\name    PeriodicTC.
*\*\fun     PeriodicTC.
*\*\param   none
*\*\return  AUDIO_OK if all operations succeed, AUDIO_FAIL else.
**/
static uint8_t PeriodicTC(uint8_t cmd)
{
    return AUDIO_OK;
}

/**
*\*\name    GetState.
*\*\fun     Return the current state of the audio machine
*\*\param   none
*\*\return  Current State.
**/
static uint8_t  GetState(void)
{
    return AudioState;
}
