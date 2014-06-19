#ifndef _BUNDLE_H_
#define _BUNDLE_H_

#include "uri.h"

#define BUNDLE_PAYLOAD_MAXLEN 	4096
#define BUNDLE_MAXLEN		8192
#define BUNDLE_DICTIONARY_BYTE_ARRAY_MAXLEN 2048

// Qos and Report not impl
enum BUNDLE_PROC_FLAG{
	BUNDLE_PROC_FLAG_FRAGMENT = 0x000001,
	BUNDLE_PROC_FLAG_ADMIN_RECORD = 0x000002,
	BUNDLE_PROC_FLAG_NOT_FRAGMENTED = 0x000004,
	BUNDLE_PROC_FLAG_CUSTODY_TRANSFER = 0x000008,
	BUNDLE_PROC_FLAG_DEST_SINGLETON = 0x000010,
	BUNDLE_PROC_FLAG_APP_ACK = 0x000020,
	BUNDLE_PROC_FLAG_RESERVED = 0x000040,
};

enum BLOCK_PROC_FLAG{
	BLOCK_PROC_FLAG_RELICATED = 0x01,
	BLOCK_PROC_FLAG_STATUS_REPORT = 0x02,
	BLOCK_PROC_FLAG_DELETE_BUNDLE = 0x04,
	BLOCK_PROC_FLAG_LAST_BLOCK = 0x08,
	BLOCK_PROC_FLAG_DISCARD_BLOCK = 0x10,
	BLOCK_PROC_FLAG_WITHOUT_PROCESSING = 0x20,
	BLOCK_PROC_FLAG_EID_REFERENCE = 0x40,
};

enum ADMIN_RECORD_TYPE{
	ADMIN_RECORD_TYPE_RESERVED = 0,
	ADMIN_RECORD_TYPE_STATUS_REPORT = 1,
	ADMIN_RECORD_TYPE_CUSTODY_SIGNAL = 2,
};

enum ADMIN_RECORD_FLAG{
	ADMIN_RECORD_FLAG_RESERVED = 0,
	ADMIN_RECORD_FLAG_FRAGMENT = 1,
};

enum CUSTODY_SIGNAL_REASON_CODE{
	CUSTODY_SIGNAL_REASON_CODE_NO_ADDITION = 0,
	CUSTODY_SIGNAL_REASON_CODE_RESERVED = 1,
	// CUSTODY_SIGNAL_REASON_CODE_RESERVED = 2,
	CUSTODY_SIGNAL_REASON_CODE_REDUNDANT = 3,
	CUSTODY_SIGNAL_REASON_CODE_DEPLETED = 4,
	CUSTODY_SIGNAL_REASON_CODE_DESTINATION_UNINTELLIGIBLE = 5,
	CUSTODY_SIGNAL_REASON_CODE_NO_ROUTE = 6,
	CUSTODY_SIGNAL_REASON_CODE_NO_CONTACT = 7,
	CUSTODY_SIGNAL_REASON_CODE_BLOCK_UNINTELLIGIBLE = 8,
};

#define BUNDLE_BODY \
	unsigned int version; \
	unsigned int bundle_proc_flags; \
	int iscustody; \
	int isadmin; \
	unsigned int primary_block_length; \
	struct URI dst_bp_endpoint_id; \
	unsigned int dst_scheme_offset; \
	unsigned int dst_ssp_offset; \
	struct URI src_bp_endpoint_id; \
	unsigned int src_scheme_offset; \
	unsigned int src_ssp_offset; \
	struct URI report_to_bp_endpoint_id; \
	unsigned int report_to_scheme_offset; \
	unsigned int report_to_ssp_offset; \
	struct URI custodian_bp_endpoint_id; \
	unsigned int custodian_scheme_offset; \
	unsigned int custodian_ssp_offset; \
	unsigned int creation_time; \
	unsigned int creation_sequence_number; \
	unsigned int lifetime; \
	unsigned int dictionary_length; \
	char dictionary_byte_array[BUNDLE_DICTIONARY_BYTE_ARRAY_MAXLEN]; \
	unsigned int block_type; \
	unsigned int block_proc_flags; \
	unsigned int payload_block_length; \
	char payload[BUNDLE_PAYLOAD_MAXLEN]; \
	unsigned int bundle_len; \
	char bundle[BUNDLE_MAXLEN]; 

#define CUSTODY_SIGNAL_BODY \
	char status; \
	unsigned int time_of_signal_sec; \
	unsigned int time_of_signal_nsec; \
	unsigned int creation_time; \
	unsigned int creation_sequence_number; \
	unsigned int src_endpoint_id_len; \
	struct URI src_endpoint_id; \
	unsigned int payload_len; \
	char payload[BUNDLE_PAYLOAD_MAXLEN];

#define STATUS_REPORT_BODY \
	;

struct BUNDLE{
	BUNDLE_BODY
};

struct CUSTODY_SIGNAL{
	char type_and_flag;
	CUSTODY_SIGNAL_BODY
};

struct STATUS_REPORT{
	char type_and_flag; 
	STATUS_REPORT_BODY
};

struct ADMIN_RECORD{
	char type_and_flag; 
	union {
		struct CUSTODY_SIGNAL custody_signal;
		struct STATUS_REPORT status_report;
	};
};

unsigned int bundle_get_version(struct BUNDLE* bundle_ptr);
void bundle_set_version(struct BUNDLE* bundle_ptr,
        unsigned int version);
void bundle_clear_bundle_proc_flags(struct BUNDLE* bundle_ptr);
int bundle_is_fragment(struct BUNDLE* bundle_ptr);
void bundle_set_fragment(struct BUNDLE* bundle_ptr);
int bundle_is_admin_record(struct BUNDLE* bundle_ptr);
void bundle_set_admin_record(struct BUNDLE* bundle_ptr);
int bundle_is_not_fragmented(struct BUNDLE* bundle_ptr);
void bundle_set_not_fragmented(struct BUNDLE* bundle_ptr);
int bundle_is_custody_transfer(struct BUNDLE* bundle_ptr);
void bundle_set_custody_transfer(struct BUNDLE* bundle_ptr);
int bundle_is_dest_singleton(struct BUNDLE* bundle_ptr);
void bundle_set_dest_singleton(struct BUNDLE* bundle_ptr);
int bundle_is_app_ack(struct BUNDLE* bundle_ptr);
void bundle_set_app_ack(struct BUNDLE* bundle_ptr);
int bundle_is_reserved(struct BUNDLE* bundle_ptr);
void bundle_set_reserved(struct BUNDLE* bundle_ptr);
unsigned int bundle_get_primary_block_length(struct BUNDLE* bundle_ptr);
void bundle_set_primary_block_length(struct BUNDLE* bundle_ptr,
        unsigned int primary_block_length);
struct URI* bundle_get_destination_uri(struct BUNDLE* bundle_ptr);
void bundle_set_destination_uri(struct BUNDLE* bundle_ptr,
        struct URI* dst_bp_endpoint_id);
struct URI* bundle_get_source_uri(struct BUNDLE* bundle_ptr);
void bundle_set_source_uri(struct BUNDLE* bundle_ptr,
        struct URI* src_bp_endpoint_id);
struct URI* bundle_get_report_to_uri(struct BUNDLE* bundle_ptr);
void bundle_set_report_to_uri(struct BUNDLE* bundle_ptr,
        struct URI* report_to_bp_endpoint_id);
struct URI* bundle_get_custodian_uri(struct BUNDLE* bundle_ptr);
void bundle_set_custodian_uri(struct BUNDLE* bundle_ptr,
        struct URI* custodian_bp_endpoint_id);
unsigned int bundle_get_creation_time(struct BUNDLE* bundle_ptr);
void bundle_set_creation_time(struct BUNDLE* bundle_ptr,
        unsigned int creation_time);
unsigned int bundle_get_creation_sequence_number(struct BUNDLE* bundle_ptr);
void bundle_set_creation_sequence_number(struct BUNDLE* bundle_ptr,
        unsigned int creation_sequence_number);
unsigned int bundle_get_lifetime(struct BUNDLE* bundle_ptr);
void bundle_set_lifetime(struct BUNDLE* bundle_ptr,
        unsigned int lifetime);
unsigned int bundle_get_dictionary_length(struct BUNDLE* bundle_ptr);
void bundle_set_dictionary_length(struct BUNDLE* bundle_ptr,
        unsigned int dictionary_length);
unsigned int bundle_get_block_type(struct BUNDLE* bundle_ptr);
void bundle_set_block_type(struct BUNDLE* bundle_ptr,
        unsigned int block_type);
void bundle_clear_block_proc_flags(struct BUNDLE* bundle_ptr);
int bunle_is_block_relicated(struct BUNDLE* bundle_ptr);
void bundle_set_block_relicated(struct BUNDLE* bundle_ptr);
int bunle_is_block_status_report(struct BUNDLE* bundle_ptr);
void bundle_set_block_status_report(struct BUNDLE* bundle_ptr);
int bunle_is_block_delete_bundle(struct BUNDLE* bundle_ptr);
void bundle_set_block_delete_bundle(struct BUNDLE* bundle_ptr);
int bunle_is_block_last_block(struct BUNDLE* bundle_ptr);
void bundle_set_block_last_block(struct BUNDLE* bundle_ptr);
int bunle_is_block_discard_block(struct BUNDLE* bundle_ptr);
void bundle_set_block_discard_block(struct BUNDLE* bundle_ptr);
int bunle_is_block_without_processing(struct BUNDLE* bundle_ptr);
void bundle_set_block_without_processing(struct BUNDLE* bundle_ptr);
int bunle_is_block_eid_reference(struct BUNDLE* bundle_ptr);
void bundle_set_block_eid_reference(struct BUNDLE* bundle_ptr);
unsigned int bundle_get_payload_length(struct BUNDLE* bundle_ptr);
void bundle_set_payload_length(struct BUNDLE* bundle_ptr,
        unsigned int payload_length);



int sdnv_encode(void* to, void* from, int from_len);
int sdnv_decode(void* to, void* from, int* from_len_ptr);
int sdnv_encode_len(void* from, int from_len);

void bundle_encode(struct BUNDLE* bundle);
void bundle_decode(struct BUNDLE* bundle);
void bundle_print(struct BUNDLE* bundle_ptr);



void admin_record_clr_type(struct ADMIN_RECORD* admin_record_ptr);
enum ADMIN_RECORD_TYPE admin_record_get_type( 
	struct ADMIN_RECORD* admin_record_ptr);
void admin_record_set_type(struct ADMIN_RECORD* admin_record_ptr,
	enum ADMIN_RECORD_TYPE type);
void admin_record_clr_flag(struct ADMIN_RECORD* admin_record_ptr);
enum ADMIN_RECORD_FLAG admin_record_get_flag(
	struct ADMIN_RECORD* admin_record_ptr);
void admin_record_set_flag(struct ADMIN_RECORD* admin_record_ptr,
	enum ADMIN_RECORD_FLAG flag);

int admin_record_is_status_report(struct ADMIN_RECORD* admin_record_ptr);
void admin_record_set_status_report(struct ADMIN_RECORD* admin_record_ptr);
int admin_record_is_custody_signal(struct ADMIN_RECORD* admin_record_ptr);
void admin_record_set_custody_signal(struct ADMIN_RECORD* admin_record_ptr);
int admin_record_is_reserved(struct ADMIN_RECORD* admin_record_ptr);
void admin_record_set_reserved(struct ADMIN_RECORD* admin_record_ptr);



int custody_signal_is_succeeded_flag(
        struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_set_succeeded_flag(
        struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_clr_succeeded_flag(
        struct CUSTODY_SIGNAL* custody_signal_ptr);
int custody_signal_is_no_addition(struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_set_no_addition(struct CUSTODY_SIGNAL* custody_signal_ptr);
int custody_signal_is_reserved(struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_set_reserved(struct CUSTODY_SIGNAL* custody_signal_ptr);
int custody_signal_is_redundant(struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_set_redundant(struct CUSTODY_SIGNAL* custody_signal_ptr);
int custody_signal_is_depleted(struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_set_depleted(struct CUSTODY_SIGNAL* custody_signal_ptr);
int custody_signal_is_destination_unintelligible(
        struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_set_destination_unintelligible(
        struct CUSTODY_SIGNAL* custody_signal_ptr);
int custody_signal_is_no_route(struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_set_no_route(struct CUSTODY_SIGNAL* custody_signal_ptr);
int custody_signal_is_no_contact(struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_set_no_contact(struct CUSTODY_SIGNAL* custody_signal_ptr);
int custody_signal_is_block_unintelligible(
        struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_set_block_unintelligible(
        struct CUSTODY_SIGNAL* custody_signal_ptr);


void custody_signal_encode(struct CUSTODY_SIGNAL* custody_signal_ptr);
void custody_signal_decode(struct CUSTODY_SIGNAL* custody_signal_ptr);









#endif

