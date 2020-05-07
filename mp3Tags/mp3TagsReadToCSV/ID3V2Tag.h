#pragma once
#include <cstdint>

//  ID3V2
//	+---------------------------- - +
//	| Header(10 bytes)				|
//	+---------------------------- - +
//	| Extended Header				|
//	| (variable length, OPTIONAL)	|
//	+---------------------------- - +
//	| Frames(variable length)		|
//	+---------------------------- - +
//	| Padding						|
//	| (variable length, OPTIONAL)	|
//	+---------------------------- - +
//	| Footer(10 bytes, OPTIONAL)	|
//	+---------------------------- - +

#pragma pack(push, 1)
struct ID3V2PLD {
	uint8_t enc;
	char txt;
};

struct ID3V2FRM {
	char id[4];
	uint8_t payloadSize[4];
	uint8_t flags[2];
	ID3V2PLD payload;
};

struct ID3V2HDR {
	char id[3];
	uint8_t ver[2];
	uint8_t flags;
	uint8_t size[4];
	ID3V2FRM firstFrame;	//assuming no extended header is present
};//supposed to be 10 bytes repeated at the end of the audio data for speed

struct ID3V2TAG {
	ID3V2HDR hdr;
	ID3V2HDR firstFrame;
};

#pragma pack(pop)

enum ID3_FrameID
{
	/* ???? */ ID3FID_NONE = 0,       /**< No known frame */
	/* AENC */ ID3FID_AUDIOCRYPTO,       /**< Audio encryption */
	/* APIC */ ID3FID_PICTURE,           /**< Attached picture */
	/* ASPI */ ID3FID_AUDIOSEEKPOINT,    /**< Audio seek point index */
	/* COMM */ ID3FID_COMMENT,           /**< Comments */
	/* COMR */ ID3FID_COMMERCIAL,        /**< Commercial frame */
	/* ENCR */ ID3FID_CRYPTOREG,         /**< Encryption method registration */
	/* EQU2 */ ID3FID_EQUALIZATION2,     /**< Equalisation (2) */
	/* EQUA */ ID3FID_EQUALIZATION,      /**< Equalization */
	/* ETCO */ ID3FID_EVENTTIMING,       /**< Event timing codes */
	/* GEOB */ ID3FID_GENERALOBJECT,     /**< General encapsulated object */
	/* GRID */ ID3FID_GROUPINGREG,       /**< Group identification registration */
	/* IPLS */ ID3FID_INVOLVEDPEOPLE,    /**< Involved people list */
	/* LINK */ ID3FID_LINKEDINFO,        /**< Linked information */
	/* MCDI */ ID3FID_CDID,              /**< Music CD identifier */
	/* MLLT */ ID3FID_MPEGLOOKUP,        /**< MPEG location lookup table */
	/* OWNE */ ID3FID_OWNERSHIP,         /**< Ownership frame */
	/* PRIV */ ID3FID_PRIVATE,           /**< Private frame */
	/* PCNT */ ID3FID_PLAYCOUNTER,       /**< Play counter */
	/* POPM */ ID3FID_POPULARIMETER,     /**< Popularimeter */
	/* POSS */ ID3FID_POSITIONSYNC,      /**< Position synchronisation frame */
	/* RBUF */ ID3FID_BUFFERSIZE,        /**< Recommended buffer size */
	/* RVA2 */ ID3FID_VOLUMEADJ2,        /**< Relative volume adjustment (2) */
	/* RVAD */ ID3FID_VOLUMEADJ,         /**< Relative volume adjustment */
	/* RVRB */ ID3FID_REVERB,            /**< Reverb */
	/* SEEK */ ID3FID_SEEKFRAME,         /**< Seek frame */
	/* SIGN */ ID3FID_SIGNATURE,         /**< Signature frame */
	/* SYLT */ ID3FID_SYNCEDLYRICS,      /**< Synchronized lyric/text */
	/* SYTC */ ID3FID_SYNCEDTEMPO,       /**< Synchronized tempo codes */
	/* TALB */ ID3FID_ALBUM,             /**< Album/Movie/Show title */
	/* TBPM */ ID3FID_BPM,               /**< BPM (beats per minute) */
	/* TCOM */ ID3FID_COMPOSER,          /**< Composer */
	/* TCON */ ID3FID_CONTENTTYPE,       /**< Content type */
	/* TCOP */ ID3FID_COPYRIGHT,         /**< Copyright message */
	/* TDAT */ ID3FID_DATE,              /**< Date */
	/* TDEN */ ID3FID_ENCODINGTIME,      /**< Encoding time */
	/* TDLY */ ID3FID_PLAYLISTDELAY,     /**< Playlist delay */
	/* TDOR */ ID3FID_ORIGRELEASETIME,   /**< Original release time */
	/* TDRC */ ID3FID_RECORDINGTIME,     /**< Recording time */
	/* TDRL */ ID3FID_RELEASETIME,       /**< Release time */
	/* TDTG */ ID3FID_TAGGINGTIME,       /**< Tagging time */
	/* TIPL */ ID3FID_INVOLVEDPEOPLE2,   /**< Involved people list */
	/* TENC */ ID3FID_ENCODEDBY,         /**< Encoded by */
	/* TEXT */ ID3FID_LYRICIST,          /**< Lyricist/Text writer */
	/* TFLT */ ID3FID_FILETYPE,          /**< File type */
	/* TIME */ ID3FID_TIME,              /**< Time */
	/* TIT1 */ ID3FID_CONTENTGROUP,      /**< Content group description */
	/* TIT2 */ ID3FID_TITLE,             /**< Title/songname/content description */
	/* TIT3 */ ID3FID_SUBTITLE,          /**< Subtitle/Description refinement */
	/* TKEY */ ID3FID_INITIALKEY,        /**< Initial key */
	/* TLAN */ ID3FID_LANGUAGE,          /**< Language(s) */
	/* TLEN */ ID3FID_SONGLEN,           /**< Length */
	/* TMCL */ ID3FID_MUSICIANCREDITLIST,/**< Musician credits list */
	/* TMED */ ID3FID_MEDIATYPE,         /**< Media type */
	/* TMOO */ ID3FID_MOOD,              /**< Mood */
	/* TOAL */ ID3FID_ORIGALBUM,         /**< Original album/movie/show title */
	/* TOFN */ ID3FID_ORIGFILENAME,      /**< Original filename */
	/* TOLY */ ID3FID_ORIGLYRICIST,      /**< Original lyricist(s)/text writer(s) */
	/* TOPE */ ID3FID_ORIGARTIST,        /**< Original artist(s)/performer(s) */
	/* TORY */ ID3FID_ORIGYEAR,          /**< Original release year */
	/* TOWN */ ID3FID_FILEOWNER,         /**< File owner/licensee */
	/* TPE1 */ ID3FID_LEADARTIST,        /**< Lead performer(s)/Soloist(s) */
	/* TPE2 */ ID3FID_BAND,              /**< Band/orchestra/accompaniment */
	/* TPE3 */ ID3FID_CONDUCTOR,         /**< Conductor/performer refinement */
	/* TPE4 */ ID3FID_MIXARTIST,         /**< Interpreted, remixed, or otherwise modified by */
	/* TPOS */ ID3FID_PARTINSET,         /**< Part of a set */
	/* TPRO */ ID3FID_PRODUCEDNOTICE,    /**< Produced notice */
	/* TPUB */ ID3FID_PUBLISHER,         /**< Publisher */
	/* TRCK */ ID3FID_TRACKNUM,          /**< Track number/Position in set */
	/* TRDA */ ID3FID_RECORDINGDATES,    /**< Recording dates */
	/* TRSN */ ID3FID_NETRADIOSTATION,   /**< Internet radio station name */
	/* TRSO */ ID3FID_NETRADIOOWNER,     /**< Internet radio station owner */
	/* TSIZ */ ID3FID_SIZE,              /**< Size */
	/* TSOA */ ID3FID_ALBUMSORTORDER,    /**< Album sort order */
	/* TSOP */ ID3FID_PERFORMERSORTORDER,/**< Performer sort order */
	/* TSOT */ ID3FID_TITLESORTORDER,    /**< Title sort order */
	/* TSRC */ ID3FID_ISRC,              /**< ISRC (international standard recording code) */
	/* TSSE */ ID3FID_ENCODERSETTINGS,   /**< Software/Hardware and settings used for encoding */
	/* TSST */ ID3FID_SETSUBTITLE,       /**< Set subtitle */
	/* TXXX */ ID3FID_USERTEXT,          /**< User defined text information */
	/* TYER */ ID3FID_YEAR,              /**< Year */
	/* UFID */ ID3FID_UNIQUEFILEID,      /**< Unique file identifier */
	/* USER */ ID3FID_TERMSOFUSE,        /**< Terms of use */
	/* USLT */ ID3FID_UNSYNCEDLYRICS,    /**< Unsynchronized lyric/text transcription */
	/* WCOM */ ID3FID_WWWCOMMERCIALINFO, /**< Commercial information */
	/* WCOP */ ID3FID_WWWCOPYRIGHT,      /**< Copyright/Legal infromation */
	/* WOAF */ ID3FID_WWWAUDIOFILE,      /**< Official audio file webpage */
	/* WOAR */ ID3FID_WWWARTIST,         /**< Official artist/performer webpage */
	/* WOAS */ ID3FID_WWWAUDIOSOURCE,    /**< Official audio source webpage */
	/* WORS */ ID3FID_WWWRADIOPAGE,      /**< Official internet radio station homepage */
	/* WPAY */ ID3FID_WWWPAYMENT,        /**< Payment */
	/* WPUB */ ID3FID_WWWPUBLISHER,      /**< Official publisher webpage */
	/* WXXX */ ID3FID_WWWUSER,           /**< User defined URL link */
};


struct ID3_FrameDef
{
	ID3_FrameID   eID;
	char          sLongTextID[4 + 1];
	const char* sDescription;
};


static  ID3_FrameDef ID3_FrameDefs[] =
{
{ID3FID_NONE,			"NONE", "No Frame"},
{ID3FID_AUDIOCRYPTO,	"AENC", "Audio encryption"},
{ID3FID_PICTURE,		"APIC", "Attached picture"},
{ID3FID_COMMENT,		"COMM", "Comments"},
{ID3FID_COMMERCIAL,		"COMR", "Commercial"},
{ID3FID_CRYPTOREG,		"ENCR", "Encryption method registration"},
{ID3FID_EQUALIZATION,	"EQUA", "Equalization"},
{ID3FID_EVENTTIMING,	"ETCO", "Event timing codes"},
{ID3FID_GENERALOBJECT,	"GEOB", "General encapsulated object"},
{ID3FID_GROUPINGREG,	"GRID", "Group identification registration"},
{ID3FID_INVOLVEDPEOPLE, "IPLS", "Involved people list"},
{ID3FID_LINKEDINFO,		"LINK", "Linked information"},
{ID3FID_CDID,			"MCDI", "Music CD identifier"},
{ID3FID_MPEGLOOKUP,		"MLLT", "MPEG location lookup table"},
{ID3FID_OWNERSHIP,		"OWNE", "Ownership frame"},
{ID3FID_PRIVATE,		"PRIV", "Private frame"},
{ID3FID_PLAYCOUNTER,	"PCNT", "Play counter"},
{ID3FID_POPULARIMETER,	"POPM", "Popularimeter"},
{ID3FID_POSITIONSYNC,	"POSS", "Position synchronisation frame"},
{ID3FID_BUFFERSIZE,		"RBUF", "Recommended buffer size"},
{ID3FID_VOLUMEADJ,		"RVAD", "Relative volume adjustment"},
{ID3FID_REVERB,			"RVRB", "Reverb"},
{ID3FID_SYNCEDLYRICS,	"SYLT", "Synchronized lyric/text"},
{ID3FID_SYNCEDTEMPO,	"SYTC", "Synchronized tempo codes"},
{ID3FID_ALBUM,			"TALB",	"Album/Movie/Show title"},
{ID3FID_BPM,			"TBPM", "BPM (beats per minute)"},
{ID3FID_COMPOSER,		"TCOM", "Composer"},
{ID3FID_CONTENTTYPE,	"TCON", "Content type"},
{ID3FID_COPYRIGHT,		"TCOP", "Copyright message"},
{ID3FID_DATE,			"TDAT", "Date"},
{ID3FID_PLAYLISTDELAY,	"TDLY", "Playlist delay"},
{ID3FID_ENCODEDBY,		"TENC", "Encoded by"},
{ID3FID_LYRICIST,		"TEXT", "Lyricist/Text writer"},
{ID3FID_FILETYPE,		"TFLT", "File type"},
{ID3FID_TIME,			"TIME", "Time"},
{ID3FID_CONTENTGROUP,	"TIT1", "Content group description"},
{ID3FID_TITLE,			"TIT2", "Title/songname/content description"},
{ID3FID_SUBTITLE,		"TIT3", "Subtitle/Description refinement"},
{ID3FID_INITIALKEY,		"TKEY", "Initial key"},
{ID3FID_LANGUAGE,		"TLAN", "Language(s)"},
{ID3FID_SONGLEN,		"TLEN", "Length"},
{ID3FID_MEDIATYPE,		"TMED", "Media type"},
{ID3FID_ORIGALBUM,		"TOAL", "Original album/movie/show title"},
{ID3FID_ORIGFILENAME,	"TOFN", "Original filename"},
{ID3FID_ORIGLYRICIST,	"TOLY", "Original lyricist(s)/text writer(s)"},
{ID3FID_ORIGARTIST,		"TOPE", "Original artist(s)/performer(s)"},
{ID3FID_ORIGYEAR,		"TORY", "Original release year"},
{ID3FID_FILEOWNER,		"TOWN", "File owner/licensee"},
{ID3FID_LEADARTIST,		"TPE1", "Lead performer(s)/Soloist(s)"},
{ID3FID_BAND,			"TPE2", "Band/orchestra/accompaniment"},
{ID3FID_CONDUCTOR,		"TPE3", "Conductor/performer refinement"},
{ID3FID_MIXARTIST,		"TPE4", "Interpreted"},
{ID3FID_PARTINSET,		"TPOS", "Part of a set"},
{ID3FID_PUBLISHER,		"TPUB", "Publisher"},
{ID3FID_TRACKNUM,		"TRCK", "Track number/Position in set"},
{ID3FID_RECORDINGDATES, "TRDA", "Recording dates"},
{ID3FID_NETRADIOSTATION,"TRSN", "Internet radio station name"},
{ID3FID_NETRADIOOWNER,	"TRSO", "Internet radio station owner"},
{ID3FID_SIZE,			"TSIZ", "Size"},
{ID3FID_ISRC,			"TSRC", "ISRC (international standard recording code)"},
{ID3FID_ENCODERSETTINGS,"TSSE", "Software/Hardware and settings used for encoding"},
{ID3FID_USERTEXT,		"TXXX", "User defined text information"},
{ID3FID_YEAR,			"TYER", "Year"},
{ID3FID_UNIQUEFILEID,	"UFID", "Unique file identifier"},
{ID3FID_TERMSOFUSE,		"USER", "Terms of use"},
{ID3FID_UNSYNCEDLYRICS, "USLT", "Unsynchronized lyric/text transcription"},
{ID3FID_WWWCOMMERCIALINFO, "WCOM", "Commercial information"},
{ID3FID_WWWCOPYRIGHT,		"WCOP", "Copyright/Legal infromation"},
{ID3FID_WWWAUDIOFILE,		"WOAF", "Official audio file webpage"},
{ID3FID_WWWARTIST,			"WOAR", "Official artist/performer webpage"},
{ID3FID_WWWAUDIOSOURCE,		"WOAS", "Official audio source webpage"},
{ID3FID_WWWRADIOPAGE,		"WORS", "Official internet radio station homepage"},
{ID3FID_WWWPAYMENT,			"WPAY", "Payment"},
{ID3FID_WWWPUBLISHER,		"WPUB", "Official publisher webpage"},
{ID3FID_WWWUSER,			"WXXX", "User defined URL link"},
};

static const uint8_t ENC_ASCII = 0;
static const uint8_t ENC_UTF16LEWBOM = 1;
static const uint8_t UTF16BEWBOM = 2;
static const uint8_t UTF16BE = 3;
static const uint8_t UTF8 = 4;

const uint8_t* getTextAddress(const ID3V2FRM* frame);
int calcID3FieldSize(const uint8_t* number);
int GetID3v2PayloadSize(const ID3V2FRM* frame);
bool isIdValid(const ID3V2FRM* frame);