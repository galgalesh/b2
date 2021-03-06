//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define ENAME int
NBEGIN(AVMediaType)
NN(AVMEDIA_TYPE_UNKNOWN)
NN(AVMEDIA_TYPE_VIDEO)
NN(AVMEDIA_TYPE_AUDIO)
NN(AVMEDIA_TYPE_DATA)
NN(AVMEDIA_TYPE_SUBTITLE)
NN(AVMEDIA_TYPE_ATTACHMENT)
NN(AVMEDIA_TYPE_NB)
NEND()
#undef ENAME

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define ENAME uint32_t
NBEGIN(AVCodecFlag)
NN(AV_CODEC_FLAG_UNALIGNED)
NN(AV_CODEC_FLAG_QSCALE)
NN(AV_CODEC_FLAG_4MV)
NN(AV_CODEC_FLAG_OUTPUT_CORRUPT)
NN(AV_CODEC_FLAG_QPEL)
NN(AV_CODEC_FLAG_PASS1)
NN(AV_CODEC_FLAG_PASS2)
NN(AV_CODEC_FLAG_LOOP_FILTER)
NN(AV_CODEC_FLAG_GRAY)
NN(AV_CODEC_FLAG_PSNR)
NN(AV_CODEC_FLAG_TRUNCATED)
NN(AV_CODEC_FLAG_INTERLACED_DCT)
NN(AV_CODEC_FLAG_LOW_DELAY)
NN(AV_CODEC_FLAG_GLOBAL_HEADER)
NN(AV_CODEC_FLAG_BITEXACT)
NN(AV_CODEC_FLAG_AC_PRED)
NN(AV_CODEC_FLAG_INTERLACED_ME)
NN(AV_CODEC_FLAG_CLOSED_GOP)
NEND()
#undef ENAME

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define ENAME uint32_t
NBEGIN(AVCodecFlag2)
NN(AV_CODEC_FLAG2_FAST)
NN(AV_CODEC_FLAG2_NO_OUTPUT)
NN(AV_CODEC_FLAG2_LOCAL_HEADER)
NN(AV_CODEC_FLAG2_DROP_FRAME_TIMECODE)
NN(AV_CODEC_FLAG2_CHUNKS)
NN(AV_CODEC_FLAG2_IGNORE_CROP)
NN(AV_CODEC_FLAG2_SHOW_ALL)
NN(AV_CODEC_FLAG2_EXPORT_MVS)
NN(AV_CODEC_FLAG2_SKIP_MANUAL)
NEND()
#undef ENAME

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define ENAME uint32_t
NBEGIN(AVCodecCap)
NN(AV_CODEC_CAP_DRAW_HORIZ_BAND)
NN(AV_CODEC_CAP_DR1)
NN(AV_CODEC_CAP_TRUNCATED)
NN(AV_CODEC_CAP_DELAY)
NN(AV_CODEC_CAP_SMALL_LAST_FRAME)
#ifdef AV_CODEC_CAP_HWACCEL_VDPAU
NN(AV_CODEC_CAP_HWACCEL_VDPAU)
#endif
NN(AV_CODEC_CAP_SUBFRAMES)
NN(AV_CODEC_CAP_EXPERIMENTAL)
NN(AV_CODEC_CAP_CHANNEL_CONF)
NN(AV_CODEC_CAP_FRAME_THREADS)
NN(AV_CODEC_CAP_SLICE_THREADS)
NN(AV_CODEC_CAP_PARAM_CHANGE)
NN(AV_CODEC_CAP_AUTO_THREADS)
NN(AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
NN(AV_CODEC_CAP_INTRA_ONLY)
NN(AV_CODEC_CAP_LOSSLESS)
NEND()
#undef ENAME

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define ENAME int
NBEGIN(AVOutputFormatFlag)
NN(AVFMT_NOFILE)
NN(AVFMT_NEEDNUMBER)
NN(AVFMT_SHOW_IDS)
#ifdef AVFMT_RAWPICTURE
NN(AVFMT_RAWPICTURE)
#endif
NN(AVFMT_GLOBALHEADER)
NN(AVFMT_NOTIMESTAMPS)
NN(AVFMT_GENERIC_INDEX)
NN(AVFMT_TS_DISCONT)
NN(AVFMT_VARIABLE_FPS)
NN(AVFMT_NODIMENSIONS)
NN(AVFMT_NOSTREAMS)
NN(AVFMT_NOBINSEARCH)
NN(AVFMT_NOGENSEARCH)
NN(AVFMT_NO_BYTE_SEEK)
NN(AVFMT_ALLOW_FLUSH)
NN(AVFMT_TS_NONSTRICT)
NN(AVFMT_TS_NEGATIVE)
NN(AVFMT_SEEK_TO_PTS)
NEND()
#undef ENAME

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
