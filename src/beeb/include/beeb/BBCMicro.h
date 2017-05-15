#ifndef HEADER_5515E213300440068BB90FD401AD3AA2
#define HEADER_5515E213300440068BB90FD401AD3AA2

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct ROM;
struct BeebKeymap;
struct DiscDriveCallbacks;
class Log;
struct VideoDataUnit;
struct SoundDataUnit;
struct DiscInterfaceDef;
class Trace;
struct TraceStats;
class TraceEventType;
class DiscImage;

#include <array>
#include <memory>
#include <vector>
#include "conf.h"
#include "crtc.h"
#include "1770.h"
#include "6522.h"
#include <6502/6502.h>
#include "SN76489.h"
#include "MC146818.h"
#include "VideoULA.h"
#include "teletext.h"
#include "DiscInterface.h"
#include <mutex>
#include <time.h>

#include <shared/enum_decl.h>
#include "BBCMicro.inl"
#include <shared/enum_end.h>

#define BBCMicroLEDFlags_AllDrives (255u*BBCMicroLEDFlag_Drive0)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// This class is a monster, and all I can do is apologise.

class BBCMicro:
    private WD1770Handler
{
public:
    union ACCCON;
    typedef void (*UpdateROMSELPagesFn)(BBCMicro *);
    typedef void (*UpdateACCCONPagesFn)(BBCMicro *,const ACCCON *);

    // nvram_contents and rtc_time are ignored if the BBCMicro doesn't
    // support such things.
    BBCMicro(BBCMicroType type,const DiscInterfaceDef *def,const std::vector<uint8_t> &nvram_contents,const tm *rtc_time);
protected:
    BBCMicro(const BBCMicro &src);
public:
    ~BBCMicro();

    // The clone has no NVRAM callbacks, no disc drive callbacks, and
    // no disc access mutex.
    std::unique_ptr<BBCMicro> Clone() const;

    typedef std::array<uint8_t,16384> ROMData;

#if BBCMICRO_TRACE
#include <shared/pshpack1.h>
    struct InstructionTraceEvent {
        uint8_t a,x,y,p,data,opcode,s;
        uint16_t pc,ad,ia;
    };
#include <shared/poppack.h>

    static const TraceEventType INSTRUCTION_EVENT;
#endif

#if BBCMICRO_TRACE
#include <shared/pshpack1.h>
    struct InitialTraceEvent {
        const struct M6502Config *config;
    };
#include <shared/poppack.h>

    static const TraceEventType INITIAL_EVENT;
#endif

#include <shared/pushwarn_bitfields.h>
    struct SystemVIAPBBits {
        uint8_t latch_index:3,latch_value:1,not_joystick0_fire:1,not_joystick1_fire:1;
    };
#include <shared/popwarn.h>

#include <shared/pushwarn_bitfields.h>
    struct BSystemVIAPBBits {
        uint8_t _:6,speech_ready:1,speech_interrupt:1;
    };
#include <shared/popwarn.h>

#include <shared/pushwarn_bitfields.h>
    struct Master128SystemVIAPBBits {
        uint8_t _:6,rtc_chip_select:1,rtc_address_strobe:1;
    };
#include <shared/popwarn.h>

    union SystemVIAPB {
        SystemVIAPBBits bits;
        BSystemVIAPBBits b_bits;
        Master128SystemVIAPBBits m128_bits;
        uint8_t value;
    };

#include <shared/pushwarn_bitfields.h>
    struct BAddressableLatchBits {
        uint8_t _:1,speech_read:1,speech_write:1;
    };
#include <shared/popwarn.h>

#include <shared/pushwarn_bitfields.h>
    struct AddressableLatchBits {
        uint8_t not_sound_write:1,_:2,not_kb_write:1,screen_base:2,caps_lock_led:1,shift_lock_led:1;
    };
#include <shared/popwarn.h>

#include <shared/pushwarn_bitfields.h>
    struct Master128AddressableLatchBits {
        uint8_t _:1,rtc_read:1,rtc_data_strobe:1;
    };
#include <shared/popwarn.h>

    union AddressableLatch {
        AddressableLatchBits bits;
        BAddressableLatchBits b_bits;
        Master128AddressableLatchBits m128_bits;
        uint8_t value;
    };

    struct BROMSELBits {
        uint8_t pr:4,_:4;
    };

    struct BPlusROMSELBits {
        uint8_t pr:4,_:3,ram:1;
    };

    struct Master128ROMSELBits {
        uint8_t pm:4,_:3,ram:1;
    };

    union ROMSEL {
        uint8_t value;
        BROMSELBits b_bits;
        BPlusROMSELBits bplus_bits;
        Master128ROMSELBits m128_bits;
    };

    struct BPlusACCCONBits {
        uint8_t _:7,shadow:1;
    };

    struct Master128ACCCONBits {
        uint8_t d:1,e:1,x:1,y:1,itu:1,ifj:1,tst:1,irr:1;
    };

    union ACCCON {
        uint8_t value;
        BPlusACCCONBits bplus_bits;
        Master128ACCCONBits m128_bits;
    };

    struct MemoryPages {
        uint8_t *w[256];
        const uint8_t *r[256];
    };

    typedef uint8_t (*ReadMMIOFn)(void *,union M6502Word);
    //struct ReadMMIO {
    //    ReadMMIOFn fn;
    //    void *obj;
    //};

    typedef void (*WriteMMIOFn)(void *,union M6502Word,uint8_t value);
    //struct WriteMMIO {
    //    WriteMMIOFn fn;
    //    void *obj;
    //};

    struct DiscDrive {
        bool motor=false;
        uint8_t track=0;
#if BBCMICRO_ENABLE_DISC_DRIVE_SOUND
        int step_sound_index=-1;

        DiscDriveSound seek_sound=DiscDriveSound_EndValue;
        size_t seek_sound_index=0;

        DiscDriveSound spin_sound=DiscDriveSound_EndValue;
        size_t spin_sound_index=0;

        float noise=0.f;
#endif
    };

    //const BBCMicroType *GetType();

    // Get read-only pointer to the cycle counter. The pointer is never
    // NULL and remains valid for the lifetime of the BBCMicro.
    //
    // (This is supposed to be impossible to get wrong (like a getter) but
    // also cheap enough in a debug build (unlike a getter) that I don't
    // have to worry about that.)
    const uint64_t *GetNum2MHzCycles();

    uint8_t GetKeyState(uint8_t key);

    // Read a value from memory. The read takes place as if the PC were in
    // zero page. There are no side-effects and reads from memory-mapped
    // I/O return 0x00.
    uint8_t ReadMemory(uint16_t address);

    // Return pointer to base of BBC RAM. Get the size of RAM from
    // GetTypeInfo(m)->ram_size.
    const uint8_t *GetRAM();

    // `key' must be a BeebKey - top bit reset. BeebSpecialKeys not
    // permitted. Returns true if the key state changed.
    int SetKeyState(uint8_t key,uint8_t new_state);

    bool HasNumericKeypad() const;

    // Sets the reset line, as linked to the Break key. To reset, set the
    // state to 1, then set it back to 0.
    void SetReset(uint8_t state);

    /* combination of DebugFlag values. */
    uint32_t GetDebugFlags();
    void SetDebugFlags(uint32_t flags);

    // *VIDEO_UNIT is always filled in. Returns true if *SOUND_UNIT
    // was filled in.
    bool Update(VideoDataUnit *video_unit,SoundDataUnit *sound_unit);

#if BBCMICRO_TURBO_DISC
    bool GetTurboDisc();
    void SetTurboDisc(int turbo);
#endif

#if BBCMICRO_ENABLE_DISC_DRIVE_SOUND
    // The disc drive sounds are used by all BBCMicro objects created
    // after they're set.
    //
    // Once a particular sound is set, it can't be changed.
    static void SetDiscDriveSound(DiscDriveType type,DiscDriveSound sound,std::vector<float> samples);
#endif

    // When a callback is set, before SetNVRAMCallback returns it is
    // immediately called with the current NVRAM contents.
    typedef void (*NVRAMChangedFn)(BBCMicro *m,size_t offset,uint8_t new_value,void *context);
    void SetNVRAMCallback(NVRAMChangedFn nvram_changed_fn,void *context);

    uint32_t GetLEDs();

    // Get the buffer size from GetNVRAMSize.
    const uint8_t *GetNVRAM();
    size_t GetNVRAMSize() const;

    // The shared_ptr is copied.
    void SetOSROM(std::shared_ptr<const ROMData> data);

    // The shared_ptr is copied.
    void SetSidewaysROM(uint8_t bank,std::shared_ptr<const ROMData> data);

    // The ROM data is copied.
    void SetSidewaysRAM(uint8_t bank,std::shared_ptr<const ROMData> data);

#if BBCMICRO_TRACE
    /* Allocates a new trace (replacing any existing one) and sets it
    * going. */
    void StartTrace(uint32_t trace_flags);

    /* If there's a trace, stops it, and returns a shared_ptr to it.
     * Otherwise, returns null. */
    std::shared_ptr<Trace> StopTrace();

    typedef void (*OnInstructionTraceEventFn)(BBCMicro *m,const InstructionTraceEvent *event,void *context);
    void SetInstructionTraceEventFn(OnInstructionTraceEventFn fn,void *context);

    int GetTraceStats(struct TraceStats *stats);
#endif

    void SetMMIOFns(uint16_t addr,ReadMMIOFn read_fn,WriteMMIOFn write_fn,void *context);
    void SetMMIOCycleStretch(uint16_t addr,bool stretch);

    std::shared_ptr<DiscImage> GetMutableDiscImage(std::unique_lock<std::mutex> *lock,int drive);
    std::shared_ptr<const DiscImage> GetDiscImage(std::unique_lock<std::mutex> *lock,int drive) const;

    void SetDiscImage(int drive,std::shared_ptr<DiscImage> disc_image);

    // Sets a pointer to the disc mutex. When non-NULL, the disc mutex
    // will be locked when changing disc image, during disc access,
    // and (via a caller-supplied unique_lock) when retrieving a disc
    // image pointer.
    //
    // (The caller-supplied unique_lock isn't foolproof; the lifetime
    // of the return pointer can exceed that of the lock. This is OK
    // if caution is exercised.)
    void SetDiscMutex(std::mutex *mutex);

    // Every time there is a disc access, the disc access flag is set
    // to true. This call retrieves its current value, and sets it to
    // false.
    //
    // The disc mutex will be locked if there is one.
    bool GetAndResetDiscAccessFlag();
protected:
private:
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //
    // stuff that's mostly copyable using copy ctor. A few things do
    // require minor fixups afterwards.
    //
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    struct State {
        // 6845
        CRTC crtc;
        CRTC::Output crtc_last_output={};

        // Video output
        VideoULA video_ula;
        SAA5050 saa5050;
        uint8_t saa5050_byte=0;

        // 0x8000 to display shadow RAM; 0x0000 to display normal RAM.
        uint16_t shadow_select_mask=0x0000;
        uint8_t cursor_pattern=0;

        SN76489 sn76489;

        // Number of emulated 2MHz cycles elapsed. Used to regulate sound
        // output and measure (for informational purposes) time between
        // vsyncs.
        uint64_t num_2MHz_cycles=0;

        // Addressable latch.
        AddressableLatch addressable_latch={};

        // Previous values, for detecting edge transitions.
        AddressableLatch old_addressable_latch={};

        M6502 cpu={};
        uint8_t stretched_cycles_left=0;
        uint8_t resetting=0;

        R6522 system_via;
        R6522 user_via;

        ROMSEL romsel={};
        ACCCON acccon={};

        // Key states
        uint8_t key_columns[16]={};
        uint8_t key_scan_column={};
        int num_keys_down=0;

        // Disk stuff
        WD1770 fdc;
        DiscInterfaceControl disc_control={};
        DiscDrive drives[NUM_DRIVES];

        // RTC
        MC146818 rtc;

        uint64_t last_vsync_2MHz_cycles=0;
        uint64_t last_frame_2MHz_cycles=0;

        std::vector<uint8_t> ram_buffer;

        std::shared_ptr<const ROMData> os_buffer;
        std::shared_ptr<const ROMData> sideways_rom_buffers[16];
        // Each element is either 16K (i.e., copy of ROMData
        // contents), or empty (nothing). Ideally this would be
        // something like unique_ptr<ROMData>, but that isn't
        // copyable.
        std::vector<uint8_t> sideways_ram_buffers[16];

        explicit State(BBCMicroType type,const std::vector<uint8_t> &nvram_contents,const tm *rtc_time);
    };

    State m_state;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //
    // stuff that is copied, but needs special handling.
    //
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    const BBCMicroType m_type;
    DiscInterface *const m_disc_interface=nullptr;
    std::shared_ptr<DiscImage> m_disc_images[NUM_DRIVES];

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //
    // stuff that can be derived from m_state or the non-copyable stuff.
    //
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    bool m_has_rtc=false;
    void (*m_handle_cpu_data_bus_fn)(BBCMicro *)=nullptr;
    UpdateROMSELPagesFn m_update_romsel_pages_fn=nullptr;
    UpdateACCCONPagesFn m_update_acccon_pages_fn=nullptr;

    // Memory
    MemoryPages m_pages={};

    // B+ and later only - read/write pages that include the shadow
    // screen RAM.
    MemoryPages *m_shadow_pages=nullptr;

    // B+ and later only - points to the MemoryPages to use. Index
    // using the MSB of the address of the last opcode fetch.
    const MemoryPages **m_pc_pages=nullptr;//[256]

    // [0] is for page FC, [1] for FD and [2] for FE.
    //
    // Each points to 256 entries, one per byte.
    //
    // These tables are used for reads. Writes always go to the
    // hardware.
    const ReadMMIOFn *m_rmmio_fns[3]={};
    void *const *m_mmio_fn_contexts[3]={};
    const uint8_t *m_mmio_stretch[3]={};

    // Tables for pages FC/FD/FE that access the hardware - B, B+,
    // M128 when ACCCON TST=0.
    std::vector<ReadMMIOFn> m_hw_rmmio_fns[3];
    std::vector<WriteMMIOFn> m_hw_wmmio_fns[3];
    std::vector<void *> m_hw_mmio_fn_contexts[3];
    std::vector<uint8_t> m_hw_mmio_stretch[3];

    // Tables for pages FC/FD/FE that access the ROM - reads on M128
    // when ACCCON TST=1. All 3 pages behave the same in this case, so
    // there's just one set of tables.
    std::vector<ReadMMIOFn> m_rom_rmmio_fns;
    std::vector<void *> m_rom_mmio_fn_contexts;
    std::vector<uint8_t> m_rom_mmio_stretch;

    uint8_t m_romsel_mask=0;
    uint8_t m_acccon_mask=0;

    uint8_t *m_ram=nullptr;

    std::vector<float> m_disc_drive_sounds[DiscDriveSound_EndValue];

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //
    // stuff that isn't copied.
    //
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //mutable std::mutex m_disc_image_mutexes[NUM_DRIVES];

    NVRAMChangedFn m_nvram_changed_fn=nullptr;
    void *m_nvram_changed_context=nullptr;
    //DiscDriveCallbacks m_disc_drive_callbacks={};
    std::mutex *m_disc_mutex=nullptr;

    // This doesn't need to be copied. The event list records its
    // influence.
    //
    // Controlled by the disc mutex, if there is one.
    bool m_disc_access=false;

#if BBCMICRO_TRACE
    // Event trace stuff.
    //
    // m_trace holds the result of m_trace_ptr.get(), in the interests
    // of non-terrible debug build performance.
    std::shared_ptr<Trace> m_trace_ptr;
    Trace *m_trace=nullptr;
    InstructionTraceEvent *m_trace_current_instruction=nullptr;
    OnInstructionTraceEventFn m_trace_instr_fn=nullptr;
    void *m_trace_instr_context=nullptr;
    uint32_t m_trace_flags=0;
#endif

    void InitStuff();
    void SetOSPages(uint8_t dest_page,uint8_t src_page,uint8_t num_pages);
    void SetROMPages(uint8_t bank,uint8_t page,size_t src_page,size_t num_pages);
#if BBCMICRO_TRACE
    void SetTrace(std::shared_ptr<Trace> trace,uint32_t trace_flags);
#endif
    void SetPages(uint8_t page_,size_t num_pages,const uint8_t *read_data,size_t read_page_stride,uint8_t *write_data,size_t write_page_stride);
    static void UpdateBROMSELPages(BBCMicro *m);
    static void UpdateBPlusACCCONPages(BBCMicro *m,const ACCCON *old);
    static void UpdateBACCCONPages(BBCMicro *m,const ACCCON *old);
    static void UpdateBPlusROMSELPages(BBCMicro *m);
    static void UpdateMaster128ROMSELPages(BBCMicro *m);
    static bool DoesMOSUseShadow(ACCCON acccon);
    static void UpdateMaster128ACCCONPages(BBCMicro *m,const ACCCON *old_);
    void InitROMPages();
    static void Write1770ControlRegister(void *m_,M6502Word a,uint8_t value);
    static uint8_t Read1770ControlRegister(void *m_,M6502Word a);
    void CallNVRAMCallback(size_t offset,uint8_t value);
#if BBCMICRO_TRACE
    void TracePortB(SystemVIAPB pb);
#endif
    static void HandleSystemVIAB(R6522 *via,uint8_t value,uint8_t old_value,void *m_);
    static void WriteUnmappedMMIO(void *m_,M6502Word a,uint8_t value);
    static uint8_t ReadUnmappedMMIO(void *m_,M6502Word a);
    static uint8_t ReadROMMMIO(void *m_,M6502Word a);
    static uint8_t ReadROMSEL(void *m_,M6502Word a);
    static void WriteROMSEL(void *m_,M6502Word a,uint8_t value);
    static uint8_t ReadACCCON(void *m_,M6502Word a);
    static void WriteACCCON(void *m_,M6502Word a,uint8_t value);
    void UpdateKeyboardMatrix();
    void UpdateJoysticks();
    bool UpdateSound(SoundDataUnit *sound_unit);
    bool PreUpdateCPU(uint8_t num_stretch_cycles);
    static void HandleCPUDataBusMainRAMOnly(BBCMicro *m);
    static void HandleCPUDataBusWithShadowRAM(BBCMicro *m);
    void UpdateVideoHardware();
    void UpdateDisplayOutput(VideoDataHalfUnit *hu);
    static void HandleTurboRTI(M6502 *cpu);

    // 1770 handler stuff.
    bool IsTrack0() override;
    void StepOut() override;
    void StepIn() override;
    void SpinUp() override;
    void SpinDown() override;
    bool IsWriteProtected() override;
    bool GetByte(uint8_t *value,uint8_t track,uint8_t sector,size_t offset) override;
    bool SetByte(uint8_t track,uint8_t sector,size_t offset,uint8_t value) override;
    bool GetSectorDetails(uint8_t *side,size_t *size,uint8_t track,uint8_t sector,bool double_density) override;
    std::unique_lock<std::mutex> GetDiscLock() const;
    DiscDrive *GetDiscDrive(std::unique_lock<std::mutex> *disc_mutex_lock);
#if BBCMICRO_ENABLE_DISC_DRIVE_SOUND
    void InitDiscDriveSounds(DiscDriveType type);
    void StepSound(DiscDrive *dd);
    float UpdateDiscDriveSound(DiscDrive *dd);
#endif
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif