// Super Nintendo SPC music file emulator

// Game_Music_Emu 0.5.2
#ifndef SPC_EMU_H
#define SPC_EMU_H

#include "Fir_Resampler.h"
#include "Music_Emu.h"
#include "Snes_Spc.h"
#include "Spc_Dsp_Register_Map_Interface.h"

class Spc_Emu : public Music_Emu, public Spc_Dsp_Register_Map_Interface {
public:
	// The Super Nintendo hardware samples at 32kHz. Other sample rates are
	// handled by resampling the 32kHz output; emulation accuracy is not affected.
	enum { native_sample_rate = 32000 };
	
	// SPC file header (ID666 binary)
	enum { header_size = 0x100 };
	struct header_t
	{
		char tag [35];
		byte format;
		byte version;
		byte pc [2];
		byte a, x, y, psw, sp;
		byte unused [2];
		char song [32];
		char game [32];
		char dumper [16];
		char comment [32];
		byte date [11];
		byte len_secs [3];
		byte fade_msec [4];
		char author [32]; // sometimes first char should be skipped (see official SPC spec)
		byte mute_mask;
		byte emulator;
		byte unused2 [46];
	};
	
	// Header for currently loaded file
	header_t const& header() const { return *(header_t const*) file_data; }
	
	// Prevents channels and global volumes from being phase-negated
	void disable_surround( bool disable = true );
	
	static gme_type_t static_type() { return gme_spc_type; }
	
public:
	// deprecated
	Music_Emu::load;
	blargg_err_t load( header_t const& h, Data_Reader& in ) // use Remaining_Reader
			{ return load_remaining_( &h, sizeof h, in ); }
	byte const* trailer() const; // use track_info()
	long trailer_size() const;

public:
	Spc_Emu();
	~Spc_Emu();

	int  read( spc_addr_t addr, int external=0) { return apu.read(addr, external); }
	void write( spc_addr_t addr, int val, int external=0) { apu.write(addr, val, external); }
	uint8_t* ram();
	long pc();
	Spc_Dsp* dsp();
	uint8_t read_dsp(uint8_t dsp_addr);
	void write_dsp(uint8_t dsp_addr, int val);

protected:
	blargg_err_t load_mem_( byte const*, long );
	blargg_err_t track_info_( track_info_t*, int track ) const;
	blargg_err_t set_sample_rate_( long );
	blargg_err_t start_track_( int );
	blargg_err_t play_( long, sample_t* );
	blargg_err_t skip_( long );
	void mute_voices_( int );
	void set_tempo_( double );
private:
	byte const* file_data;
	long        file_size;
	Fir_Resampler<24> resampler;
	Snes_Spc apu;
};

//(spcemu..).apu.write()

inline void Spc_Emu::disable_surround( bool b ) { apu.disable_surround( b ); }
inline uint8_t* Spc_Emu::ram() { return apu.ram(); }
inline long Spc_Emu::pc() { return apu.pc(); }
inline Spc_Dsp* Spc_Emu::dsp() { return apu.get_dsp(); }
inline uint8_t Spc_Emu::read_dsp(uint8_t dsp_addr) { return apu.get_dsp()->read(dsp_addr); }
inline void Spc_Emu::write_dsp(uint8_t dsp_addr, int val) { apu.get_dsp()->write(dsp_addr, val); }

#endif
