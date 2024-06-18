#ifndef ISO9660_H
#define ISO9660_H
#include <stddef.h>
#include <stdint.h>

#define ISO_BLOCK_SIZE 2048
#define ISO_MAX_DIR_DEPTH 8

struct int16_lsb_msb
{
    uint16_t le;
    uint16_t be;
} __attribute__((packed));

struct int32_lsb_msb
{
    uint32_t le;
    uint32_t be;
} __attribute__((packed));

// See: 9.4 - Format of a Path Table Record
struct iso_path_table_le
{
    uint8_t idf_len; // Identifier name length
    uint8_t ext_size; // Extended attribute record length
    uint32_t data_blk; // File data block index
    uint16_t parent_dir; // Number of the parent dir
} __attribute__((packed));

enum iso_file_type
{
    ISO_FILE_HIDDEN = 1, // File is Hidden
    ISO_FILE_ISDIR = 2, // Entry is a Directory
    ISO_FILE_ASSOCIAT = 4, // Entry is an Associated
    ISO_FILE_USEEXT = 8,
    // Information is structured according to the extended attribute record
    ISO_FILE_USEPERM = 16,
    // Permissions are specified in the extended attribute record
    ISO_FILE_MULTIDIR = 128 // File has more than one directory record
};

// See: 9.1 - Format of a Directory Record
#define ISO_DATE_LEN 7

struct iso_dir
{
    uint8_t dir_size; // Length of directory record
    uint8_t ext_size; // Extended attribute record length
    struct int32_lsb_msb data_blk; // File data block index
    struct int32_lsb_msb file_size; // File size
    char date[ISO_DATE_LEN];
    uint8_t type; // File type (enum iso_file_type)

    // Only valid if the file is recorded in interleave mode
    uint8_t unit_size; // File Unit Size
    uint8_t gap_size; // Interleave Gap Size

    struct int16_lsb_msb vol_seq;
    uint8_t idf_len;
} __attribute__((packed));

// See: 8.4 - Primary Volume Descriptor
#define ISO_PRIM_VOLDESC_BLOCK 16

#define ISO_SYSIDF_LEN 32
#define ISO_VOLIDF_LEN 32
#define ISO_VOLSET_LEN 128
#define ISO_PUBIDF_LEN 128
#define ISO_DPREP_LEN 128
#define ISO_APP_LEN 128
#define ISO_CPRFIL_LEN 37
#define ISO_ABSFIL_LEN 37
#define ISO_BIBFIL_LEN 37
#define ISO_LDATE_LEN 17

struct iso_prim_voldesc
{
    uint8_t vol_desc_type; // Volume Descriptor Type (1)
    char std_identifier[5]; // Standard Identifier (CD001)
    uint8_t vol_desc_version; // Volume Descriptor Version (1)
    uint8_t unused1; // Unused Field
    char syidf[ISO_SYSIDF_LEN]; // System Identifier
    char vol_idf[ISO_VOLIDF_LEN]; // Volume Identifier
    uint8_t unused2[8]; // Unused Field
    struct int32_lsb_msb vol_blk_count;
    // Number of logical blocks in the Volume (LE)
    uint8_t unused4[32]; // Unused Field
    struct int16_lsb_msb vol_set_size; // The Volume Set size of the Volume
    struct int16_lsb_msb vol_seq_num; // The number of the volume in the Set
    struct int16_lsb_msb vol_blk_size; // The size in bytes of a Logical Block
    struct int32_lsb_msb path_table_size; // Length in bytes of the path table
    uint32_t le_path_table_blk; // LittleEndian path table block index
    uint32_t le_opath_table_blk;
    // LittleEndian optional path table block index
    uint32_t be_path_table_blk; // BigEndian path table block index
    uint32_t be_opath_table_blk; // BigEndian optional path table block index

    struct iso_dir root_dir; // Root directory entry
    uint8_t unused5[34 - sizeof(struct iso_dir)]; // padding

    char volset_idf[ISO_VOLSET_LEN]; // Name of the multiple volume set
    char pub_idf[ISO_PUBIDF_LEN]; // Publisher name
    char dprep_idf[ISO_DPREP_LEN]; // Data preparer name
    char app_idf[ISO_APP_LEN]; // Application name

    char copyright_file[ISO_CPRFIL_LEN]; // Copyright file name in root dir
    char abstract_file[ISO_ABSFIL_LEN]; // Abstract file name in root dir
    char bibli_file[ISO_BIBFIL_LEN]; // Bibliographic file name in root dir
    char date_creat[ISO_LDATE_LEN]; // Creation date
    char date_modif[ISO_LDATE_LEN]; // Modification date
    char date_expir[ISO_LDATE_LEN]; // Expiration date
    char date_effect[ISO_LDATE_LEN]; // Effective date
    uint8_t filestrutc_version; // File Structure Version (1)
} __attribute__((packed));

void help();
void info(struct iso_prim_voldesc *pvd);
void list_directory(struct iso_dir *dir, void *iso_data);
void display_file_content(struct iso_dir *dir, void *iso_data,
                          const char *filename);
void change_directory(struct iso_dir *current_dir, void *iso_data,
                      const char *path);
void print_working_directory(struct iso_dir *current_dir, void *iso_data);
void quit_program();
void get_file(struct iso_dir *dir, void *iso_data, const char *filename);

#endif // !ISO9660_H
