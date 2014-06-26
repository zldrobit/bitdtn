#include <stdio.h>
#include <string.h>

#include "bp.h"
#include "bundle.h"
#include "uri.h"

#define SDNV_DECODE_MAXLEN 100

#define set_bit(a, bit) {(a) |= 1 << (7-(bit));}
#define clr_bit(a, bit) {(a) &= ~(1 << (7-(bit)));}
#define ass_bit(a, bit, v) \
	if (v){ \
		set_bit(a, bit); \
	} \
	else { \
		clr_bit(a, bit); \
	} 
#define get_bit(a, bit) (!!((a) & (1 << (7-(bit)))))

void printbits(unsigned char* a, int len)
{
	int i,j;
	for (i = 0; i < len; i++){
		for (j = 0; j < 8; j++){
			if (get_bit(a[i], j)){
				printf("1");
			} 
			else {
				printf("0");
			}
		}	
		printf(" ");
	}
	printf("\n");
}

unsigned int bundle_get_version(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->version;
}

void bundle_set_version(struct BUNDLE* bundle_ptr,
	unsigned int version)
{
	bundle_ptr->version = version;
}

void bundle_clear_bundle_proc_flags(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->bundle_proc_flags = 0;
}

int bundle_is_fragment(struct BUNDLE* bundle_ptr)
{
	return !!(bundle_ptr->bundle_proc_flags & BUNDLE_PROC_FLAG_FRAGMENT);

}

void bundle_set_fragment(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->bundle_proc_flags |= BUNDLE_PROC_FLAG_FRAGMENT;
}

int bundle_is_admin_record(struct BUNDLE* bundle_ptr)
{
	return !!(bundle_ptr->bundle_proc_flags & 
		BUNDLE_PROC_FLAG_ADMIN_RECORD);
}

void bundle_set_admin_record(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->bundle_proc_flags |= 
		BUNDLE_PROC_FLAG_ADMIN_RECORD;
}

int bundle_is_not_fragmented(struct BUNDLE* bundle_ptr)
{
	return !!(bundle_ptr->bundle_proc_flags & 
		BUNDLE_PROC_FLAG_NOT_FRAGMENTED);
}

void bundle_set_not_fragmented(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->bundle_proc_flags |= 
		BUNDLE_PROC_FLAG_NOT_FRAGMENTED;
}

int bundle_is_custody_transfer(struct BUNDLE* bundle_ptr)
{
	return !!(bundle_ptr->bundle_proc_flags & 
		BUNDLE_PROC_FLAG_CUSTODY_TRANSFER);
}

void bundle_set_custody_transfer(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->bundle_proc_flags |= 
		BUNDLE_PROC_FLAG_CUSTODY_TRANSFER;
}

int bundle_is_dest_singleton(struct BUNDLE* bundle_ptr)
{
	return !!(bundle_ptr->bundle_proc_flags & 
		BUNDLE_PROC_FLAG_DEST_SINGLETON);
}

void bundle_set_dest_singleton(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->bundle_proc_flags |= 
		BUNDLE_PROC_FLAG_DEST_SINGLETON;
}

int bundle_is_app_ack(struct BUNDLE* bundle_ptr)
{
	return !!(bundle_ptr->bundle_proc_flags & 
		BUNDLE_PROC_FLAG_APP_ACK);
}

void bundle_set_app_ack(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->bundle_proc_flags |= 
		BUNDLE_PROC_FLAG_APP_ACK;
}

int bundle_is_reserved(struct BUNDLE* bundle_ptr)
{
	return !!(bundle_ptr->bundle_proc_flags & 
		BUNDLE_PROC_FLAG_RESERVED);
}

void bundle_set_reserved(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->bundle_proc_flags |= 
		BUNDLE_PROC_FLAG_RESERVED;
}

unsigned int bundle_get_primary_block_length(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->primary_block_length;
}

void bundle_set_primary_block_length(struct BUNDLE* bundle_ptr,
	unsigned int primary_block_length)
{
	bundle_ptr->primary_block_length = primary_block_length;
}

struct URI* bundle_get_destination_uri(struct BUNDLE* bundle_ptr)
{
	return &bundle_ptr->dst_bp_endpoint_id;
}

void bundle_set_destination_uri(struct BUNDLE* bundle_ptr, 
	struct URI* dst_bp_endpoint_id_ptr)
{
	uri_copy(&bundle_ptr->dst_bp_endpoint_id, dst_bp_endpoint_id_ptr);
}

struct URI* bundle_get_source_uri(struct BUNDLE* bundle_ptr)
{
	return &bundle_ptr->src_bp_endpoint_id;
}

void bundle_set_source_uri(struct BUNDLE* bundle_ptr, 
	struct URI* src_bp_endpoint_id_ptr)
{
	uri_copy(&bundle_ptr->src_bp_endpoint_id, src_bp_endpoint_id_ptr);
}

struct URI* bundle_get_report_to_uri(struct BUNDLE* bundle_ptr)
{
	return &bundle_ptr->report_to_bp_endpoint_id;
}

void bundle_set_report_to_uri(struct BUNDLE* bundle_ptr, 
	struct URI* report_to_bp_endpoint_id_ptr)
{
	uri_copy(&bundle_ptr->report_to_bp_endpoint_id, 
		report_to_bp_endpoint_id_ptr);
}

struct URI* bundle_get_custodian_uri(struct BUNDLE* bundle_ptr)
{
	return &bundle_ptr->custodian_bp_endpoint_id;
}

void bundle_set_custodian_uri(struct BUNDLE* bundle_ptr, 
	struct URI* custodian_bp_endpoint_id_ptr)
{
	uri_copy(&bundle_ptr->custodian_bp_endpoint_id, 
		custodian_bp_endpoint_id_ptr);
}

unsigned int bundle_get_creation_time(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->creation_time;
}

void bundle_set_creation_time(struct BUNDLE* bundle_ptr,
	unsigned int creation_time)
{
	bundle_ptr->creation_time = creation_time;
}

unsigned int bundle_get_creation_sequence_number(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->creation_sequence_number;
}

void bundle_set_creation_sequence_number(struct BUNDLE* bundle_ptr,
	unsigned int creation_sequence_number)
{
	bundle_ptr->creation_sequence_number = creation_sequence_number;
}

unsigned int bundle_get_lifetime(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->lifetime;
}

void bundle_set_lifetime(struct BUNDLE* bundle_ptr,
	unsigned int lifetime)
{
	bundle_ptr->lifetime = lifetime;
}

unsigned int bundle_get_dictionary_length(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->dictionary_length;
}

void bundle_set_dictionary_length(struct BUNDLE* bundle_ptr,
	unsigned int dictionary_length)
{
	bundle_ptr->dictionary_length = dictionary_length;
}

unsigned int bundle_get_block_type(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->block_type;
}

void bundle_set_block_type(struct BUNDLE* bundle_ptr,
	unsigned int block_type)
{
	bundle_ptr->block_type = block_type;
}

void bundle_clear_block_proc_flags(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->block_proc_flags = 0;
}

int bunle_is_block_relicated(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->block_proc_flags & BLOCK_PROC_FLAG_RELICATED;
}

void bundle_set_block_relicated(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->block_proc_flags |= BLOCK_PROC_FLAG_RELICATED;
}

int bunle_is_block_status_report(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->block_proc_flags & BLOCK_PROC_FLAG_STATUS_REPORT;
}

void bundle_set_block_status_report(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->block_proc_flags |= BLOCK_PROC_FLAG_STATUS_REPORT;
}

int bunle_is_block_delete_bundle(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->block_proc_flags & BLOCK_PROC_FLAG_DELETE_BUNDLE;
}

void bundle_set_block_delete_bundle(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->block_proc_flags |= BLOCK_PROC_FLAG_DELETE_BUNDLE;
}

int bunle_is_block_last_block(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->block_proc_flags & BLOCK_PROC_FLAG_LAST_BLOCK;
}

void bundle_set_block_last_block(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->block_proc_flags |= BLOCK_PROC_FLAG_LAST_BLOCK;
}

int bunle_is_block_discard_block(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->block_proc_flags & BLOCK_PROC_FLAG_DISCARD_BLOCK;
}

void bundle_set_block_discard_block(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->block_proc_flags |= BLOCK_PROC_FLAG_DISCARD_BLOCK;
}

int bunle_is_block_without_processing(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->block_proc_flags & 
		BLOCK_PROC_FLAG_WITHOUT_PROCESSING;
}

void bundle_set_block_without_processing(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->block_proc_flags |= 
		BLOCK_PROC_FLAG_WITHOUT_PROCESSING;
}

int bunle_is_block_eid_reference(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->block_proc_flags & 
		BLOCK_PROC_FLAG_EID_REFERENCE;
}

void bundle_set_block_eid_reference(struct BUNDLE* bundle_ptr)
{
	bundle_ptr->block_proc_flags |= 
		BLOCK_PROC_FLAG_EID_REFERENCE;
}

unsigned int bundle_get_payload_length(struct BUNDLE* bundle_ptr)
{
	return bundle_ptr->payload_block_length;
}

void bundle_set_payload_length(struct BUNDLE* bundle_ptr,
	unsigned int payload_block_length)
{
	bundle_ptr->payload_block_length = payload_block_length;
}

int sdnv_encode_len(void* _from, int from_len)
{
	int msbyte_digits;
	unsigned int tmp;
	int i,j;
	int offset;
	unsigned char* from;
	int to_len;
	
	from = _from;
	tmp = from[from_len-1];	
	msbyte_digits = 0;
	while (tmp > 0){
		tmp >>= 1;
		msbyte_digits++;
	}
	to_len = ((from_len-1)*8 + msbyte_digits + 6) / 7;
	if (to_len == 0){
		// to[0] = 0;
		return 1;
	}
	return to_len;
}

void bundle_encode(struct BUNDLE* bundle_ptr)
{
	int offset;
	int tmp;
	int dict_len;

	// process dictionary first
	dict_len = 0;
	bundle_ptr->dictionary_byte_array[0] = '\0';
	strcat(bundle_ptr->dictionary_byte_array,
		bundle_ptr->dst_bp_endpoint_id.scheme);	
	bundle_ptr->dst_scheme_offset = 0;
	dict_len = strlen(bundle_ptr->dst_bp_endpoint_id.scheme)+1;

	bundle_ptr->dictionary_byte_array[dict_len] = '\0';
	strcat(bundle_ptr->dictionary_byte_array+dict_len,
		bundle_ptr->dst_bp_endpoint_id.ssp);	
	bundle_ptr->dst_ssp_offset = dict_len;
	dict_len += strlen(bundle_ptr->dst_bp_endpoint_id.ssp)+1;


	bundle_ptr->dictionary_byte_array[dict_len] = '\0';
	strcat(bundle_ptr->dictionary_byte_array+dict_len,
		bundle_ptr->src_bp_endpoint_id.scheme);	
	bundle_ptr->src_scheme_offset = dict_len;
	dict_len += strlen(bundle_ptr->src_bp_endpoint_id.scheme)+1;

	bundle_ptr->dictionary_byte_array[dict_len] = '\0';
	strcat(bundle_ptr->dictionary_byte_array+dict_len,
		bundle_ptr->src_bp_endpoint_id.ssp);	
	bundle_ptr->src_ssp_offset = dict_len;
	dict_len += strlen(bundle_ptr->src_bp_endpoint_id.ssp)+1;


	bundle_ptr->dictionary_byte_array[dict_len] = '\0';
	strcat(bundle_ptr->dictionary_byte_array+dict_len,
		bundle_ptr->report_to_bp_endpoint_id.scheme);	
	bundle_ptr->report_to_scheme_offset = dict_len;
	dict_len += strlen(bundle_ptr->report_to_bp_endpoint_id.scheme)+1;

	bundle_ptr->dictionary_byte_array[dict_len] = '\0';
	strcat(bundle_ptr->dictionary_byte_array+dict_len,
		bundle_ptr->report_to_bp_endpoint_id.ssp);	
	bundle_ptr->report_to_ssp_offset = dict_len;
	dict_len += strlen(bundle_ptr->report_to_bp_endpoint_id.ssp)+1;


	bundle_ptr->dictionary_byte_array[dict_len] = '\0';
	strcat(bundle_ptr->dictionary_byte_array+dict_len,
		bundle_ptr->custodian_bp_endpoint_id.scheme);	
	bundle_ptr->custodian_scheme_offset = dict_len;
	dict_len += strlen(bundle_ptr->custodian_bp_endpoint_id.scheme)+1;

	bundle_ptr->dictionary_byte_array[dict_len] = '\0';
	strcat(bundle_ptr->dictionary_byte_array+dict_len,
		bundle_ptr->custodian_bp_endpoint_id.ssp);	
	bundle_ptr->custodian_ssp_offset = dict_len;
	dict_len += strlen(bundle_ptr->custodian_bp_endpoint_id.ssp)+1;

	bundle_ptr->dictionary_length = dict_len;

	// start of primary block
	bundle_ptr->bundle[0] = (char) (bundle_ptr->version);
	offset = 1;
	
	// bundle proc flags 
	bundle_clear_bundle_proc_flags(bundle_ptr);
	if (bundle_ptr->iscustody){
		bundle_set_custody_transfer(bundle_ptr);
	}
	if (bundle_ptr->isadmin){
		bundle_set_admin_record(bundle_ptr);
	}
	tmp = sdnv_encode(bundle_ptr->bundle + offset, 
		&bundle_ptr->bundle_proc_flags, 4);
	offset += tmp;

	bundle_ptr->primary_block_length = 
		sdnv_encode_len(&bundle_ptr->dst_scheme_offset, 4) +	
		sdnv_encode_len(&bundle_ptr->dst_ssp_offset, 4) +	
		sdnv_encode_len(&bundle_ptr->src_scheme_offset, 4) +	
		sdnv_encode_len(&bundle_ptr->src_ssp_offset, 4) +	
		sdnv_encode_len(&bundle_ptr->report_to_scheme_offset, 4) +	
		sdnv_encode_len(&bundle_ptr->report_to_ssp_offset, 4) +	
		sdnv_encode_len(&bundle_ptr->custodian_scheme_offset, 4) +	
		sdnv_encode_len(&bundle_ptr->custodian_ssp_offset, 4) +	
		sdnv_encode_len(&bundle_ptr->creation_time, 4) +
		sdnv_encode_len(&bundle_ptr->creation_sequence_number, 4) +
		sdnv_encode_len(&bundle_ptr->lifetime, 4) +
		sdnv_encode_len(&bundle_ptr->dictionary_length, 4) +
		bundle_ptr->dictionary_length;

	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->primary_block_length, 4);
	offset += tmp;

	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->dst_scheme_offset, 4);
	offset += tmp;
	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->dst_ssp_offset, 4);
	offset += tmp;

	
	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->src_scheme_offset, 4);
	offset += tmp;
	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->src_ssp_offset, 4);
	offset += tmp;

	
	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->report_to_scheme_offset, 4);
	offset += tmp;
	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->report_to_ssp_offset, 4);
	offset += tmp;
	

	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->custodian_scheme_offset, 4);
	offset += tmp;
	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->custodian_ssp_offset, 4);
	offset += tmp;
	

	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->creation_time, 4);
	offset += tmp;
	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->creation_sequence_number, 4);
	offset += tmp;

	// printf("encode lifetime offset = %d\n", offset);
	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->lifetime, 4);
	offset += tmp;

	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->dictionary_length, 4);
	offset += tmp;

	memcpy(bundle_ptr->bundle + offset, bundle_ptr->dictionary_byte_array,
		bundle_ptr->dictionary_length);
	offset += bundle_ptr->dictionary_length;

	// start of the payload block
	bundle_ptr->bundle[offset] = (char) (bundle_ptr->block_type);
	offset += 1;

	tmp = sdnv_encode(bundle_ptr->bundle + offset, 
		&bundle_ptr->block_proc_flags, 4);
	offset += tmp;
	
	tmp = sdnv_encode(bundle_ptr->bundle + offset,
		&bundle_ptr->payload_block_length, 4);
	offset += tmp;

	memcpy(bundle_ptr->bundle + offset, bundle_ptr->payload,
		bundle_ptr->payload_block_length);
	offset += bundle_ptr->payload_block_length;

	bundle_ptr->bundle_len = offset;
}

void bundle_decode(struct BUNDLE* bundle_ptr)
{
	int offset,tmp;

	bundle_ptr->version = bundle_ptr->bundle[0];
	offset = 1;

	bundle_ptr->bundle_proc_flags = 0;
	sdnv_decode(&bundle_ptr->bundle_proc_flags, 
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;

	
	sdnv_decode(&bundle_ptr->primary_block_length, 
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;

	sdnv_decode(&bundle_ptr->dst_scheme_offset,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
	sdnv_decode(&bundle_ptr->dst_ssp_offset,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
		

	sdnv_decode(&bundle_ptr->src_scheme_offset,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
	sdnv_decode(&bundle_ptr->src_ssp_offset,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
		

	sdnv_decode(&bundle_ptr->report_to_scheme_offset,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
	sdnv_decode(&bundle_ptr->report_to_ssp_offset,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
		

	sdnv_decode(&bundle_ptr->custodian_scheme_offset,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
	sdnv_decode(&bundle_ptr->custodian_ssp_offset,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
		

	sdnv_decode(&bundle_ptr->creation_time,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
	sdnv_decode(&bundle_ptr->creation_sequence_number,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;

	printf("decode lifetime offset = %d\n", offset);
	sdnv_decode(&bundle_ptr->lifetime,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;

	sdnv_decode(&bundle_ptr->dictionary_length,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;

	memcpy(bundle_ptr->dictionary_byte_array, 
		bundle_ptr->bundle + offset, 
		bundle_ptr->dictionary_length);
	offset += bundle_ptr->dictionary_length;

	// process dictionary
	strcpy(bundle_ptr->dst_bp_endpoint_id.scheme,
		bundle_ptr->dictionary_byte_array + 
		bundle_ptr->dst_scheme_offset);
	strcpy(bundle_ptr->dst_bp_endpoint_id.ssp,
		bundle_ptr->dictionary_byte_array + 
		bundle_ptr->dst_ssp_offset);

	strcpy(bundle_ptr->src_bp_endpoint_id.scheme,
		bundle_ptr->dictionary_byte_array + 
		bundle_ptr->src_scheme_offset);
	strcpy(bundle_ptr->src_bp_endpoint_id.ssp,
		bundle_ptr->dictionary_byte_array + 
		bundle_ptr->src_ssp_offset);

	strcpy(bundle_ptr->report_to_bp_endpoint_id.scheme,
		bundle_ptr->dictionary_byte_array + 
		bundle_ptr->report_to_scheme_offset);
	strcpy(bundle_ptr->report_to_bp_endpoint_id.ssp,
		bundle_ptr->dictionary_byte_array + 
		bundle_ptr->report_to_ssp_offset);

	strcpy(bundle_ptr->custodian_bp_endpoint_id.scheme,
		bundle_ptr->dictionary_byte_array + 
		bundle_ptr->custodian_scheme_offset);
	strcpy(bundle_ptr->custodian_bp_endpoint_id.ssp,
		bundle_ptr->dictionary_byte_array + 
		bundle_ptr->custodian_ssp_offset);

	// start of payload block
	bundle_ptr->block_type = bundle_ptr->bundle[offset];
	offset += 1;

	// bundle proc flags
	sdnv_decode(&bundle_ptr->block_proc_flags,
		bundle_ptr->bundle + offset, &tmp);
	offset += tmp;
	if (bundle_is_custody_transfer(bundle_ptr)){
		bundle_ptr->iscustody = 1;
	}
	else {
		bundle_ptr->iscustody = 0;
	}
	if (bundle_is_admin_record(bundle_ptr)){
		bundle_ptr->isadmin = 1;
	}
	else {
		bundle_ptr->isadmin = 0;
	}
	
	sdnv_decode(&bundle_ptr->payload_block_length,
		bundle_ptr ->bundle+ offset, &tmp);
	offset += tmp;

	memcpy(bundle_ptr->payload, 
		bundle_ptr->bundle + offset,
		bundle_ptr->payload_block_length);
	offset += bundle_ptr->payload_block_length;

	bundle_ptr->bundle_len = offset;
}

void bundle_print(struct BUNDLE* bundle_ptr)
{
	int i;

	printf("version = %d\n", bundle_ptr->version);
	printf("bundle_proc_flags = %d\n", bundle_ptr->bundle_proc_flags);
	printf("bundle is admin record = %d\n", 
		bundle_is_admin_record(bundle_ptr));
	printf("bundle iscustody = %d\n", bundle_ptr->iscustody);
	printf("primary_block_length = %d\n", 
		bundle_ptr->primary_block_length);
	printf("dst_scheme_offset = %d, dst_scheme = %s\n",
		bundle_ptr->dst_scheme_offset, 
		bundle_ptr->dst_bp_endpoint_id.scheme);
	printf("dst_ssp_offset = %d, dst_ssp = %s\n",
		bundle_ptr->dst_ssp_offset, 
		bundle_ptr->dst_bp_endpoint_id.ssp);
	printf("src_scheme_offset = %d, src_scheme = %s\n",
		bundle_ptr->src_scheme_offset, 
		bundle_ptr->src_bp_endpoint_id.scheme);
	printf("src_ssp_offset = %d, src_ssp = %s\n",
		bundle_ptr->src_ssp_offset, 
		bundle_ptr->src_bp_endpoint_id.ssp);
	printf("report_to_scheme_offset = %d, report_to_scheme = %s\n",
		bundle_ptr->report_to_scheme_offset, 
		bundle_ptr->report_to_bp_endpoint_id.scheme);
	printf("report_to_ssp_offset = %d, report_to_ssp = %s\n",
		bundle_ptr->report_to_ssp_offset, 
		bundle_ptr->report_to_bp_endpoint_id.ssp);
	printf("custodian_scheme_offset = %d, custodian_scheme = %s\n",
		bundle_ptr->custodian_scheme_offset, 
		bundle_ptr->custodian_bp_endpoint_id.scheme);
	printf("custodian_ssp_offset = %d, custodian_ssp = %s\n",
		bundle_ptr->custodian_ssp_offset, 
		bundle_ptr->custodian_bp_endpoint_id.ssp);
	printf("creation_time = %u\n", bundle_ptr->creation_time);
	printf("creation_sequence_number = %u\n", 
		bundle_ptr->creation_sequence_number);
	printf("lifetime = %d\n", bundle_ptr->lifetime);
	printf("dictionary_length = %d\n", bundle_ptr->dictionary_length);
	printf("dictionary_byte_array is:\n");
	for (i = 0; i < bundle_ptr->dictionary_length; i++){
		if (bundle_ptr->dictionary_byte_array[i] == '\0'){
			printf("0");
		}
		else {
			printf("%c", bundle_ptr->dictionary_byte_array[i]);
		}
	}
	printf("\n");

	printf("block_type = %d\n", bundle_ptr->block_type);
	printf("block_proc_flags = %d\n", bundle_ptr->block_proc_flags);
	printf("payload_block_length = %d\n", 
		bundle_ptr->payload_block_length);
	printf("payload is:\n");
	for (i = 0; i < bundle_ptr->payload_block_length; i++){
		printf("%c", bundle_ptr->payload[i]);
	}
	printf("\n");
	printf("bundle_len = %d\n", bundle_ptr->bundle_len);
	printf("\n");
	
}

void bundle_print_header(struct BUNDLE* bundle_ptr)
{
	int i;

	printf("bundle_proc_flags = %d\n", bundle_ptr->bundle_proc_flags);
	printf("bundle is admin record = %d\n", 
		bundle_is_admin_record(bundle_ptr));
	printf("bundle iscustody = %d\n", bundle_ptr->iscustody);
	printf("dst bp endpoint id = %s:%s\n",
		bundle_ptr->dst_bp_endpoint_id.scheme,
		bundle_ptr->dst_bp_endpoint_id.ssp);
	printf("src bp endpoint id = %s:%s\n",
		bundle_ptr->src_bp_endpoint_id.scheme,
		bundle_ptr->src_bp_endpoint_id.ssp);
	printf("report_to bp endpoint id = %s:%s\n",
		bundle_ptr->report_to_bp_endpoint_id.scheme,
		bundle_ptr->report_to_bp_endpoint_id.ssp);
	printf("custodian bp endpoint id = %s:%s\n",
		bundle_ptr->custodian_bp_endpoint_id.scheme,
		bundle_ptr->custodian_bp_endpoint_id.ssp);
	printf("creation_time = %u\n", bundle_ptr->creation_time);
	printf("creation_sequence_number = %u\n", 
		bundle_ptr->creation_sequence_number);
}

void bundle_print_uid(struct BUNDLE* bundle_ptr)
{
	int i;

	printf("src bp endpoint id = %s:%s\n",
		bundle_ptr->src_bp_endpoint_id.scheme,
		bundle_ptr->src_bp_endpoint_id.ssp);
	printf("creation_time = %u\n", bundle_ptr->creation_time);
	printf("creation_sequence_number = %u\n", 
		bundle_ptr->creation_sequence_number);
}

void admin_record_clr_type(struct ADMIN_RECORD* admin_record_ptr)
{
	admin_record_ptr->type_and_flag &= 0x0F; 
}

enum ADMIN_RECORD_TYPE admin_record_get_type( 
	struct ADMIN_RECORD* admin_record_ptr)
{
	return (admin_record_ptr->type_and_flag & 0xF0) >> 4;
}

void admin_record_set_type(struct ADMIN_RECORD* admin_record_ptr,
	enum ADMIN_RECORD_TYPE type)
{
	admin_record_ptr->type_and_flag &= 0x0F;
	admin_record_ptr->type_and_flag |= type << 4;
}

void admin_record_clr_flag(struct ADMIN_RECORD* admin_record_ptr)
{
	admin_record_ptr->type_and_flag &= 0xF0;
}

enum ADMIN_RECORD_FLAG admin_record_get_flag(
	struct ADMIN_RECORD* admin_record_ptr)
{
	return admin_record_ptr->type_and_flag & 0x0F;
}

void admin_record_set_flag(struct ADMIN_RECORD* admin_record_ptr,
	enum ADMIN_RECORD_FLAG flag)
{
	admin_record_ptr->type_and_flag &= 0xF0;	
	admin_record_ptr->type_and_flag |= flag;
}

int admin_record_is_status_report(struct ADMIN_RECORD* admin_record_ptr)
{
	if (admin_record_get_type(admin_record_ptr) == 
		ADMIN_RECORD_TYPE_STATUS_REPORT){
		return 1;
	}
	else {
		return 0;
	}
}

void admin_record_set_status_report(struct ADMIN_RECORD* admin_record_ptr)
{
	admin_record_set_type(admin_record_ptr, 
		ADMIN_RECORD_TYPE_STATUS_REPORT);
}

int admin_record_is_custody_signal(struct ADMIN_RECORD* admin_record_ptr)
{
	if (admin_record_get_type(admin_record_ptr) == 
		ADMIN_RECORD_TYPE_CUSTODY_SIGNAL){
		return 1;
	}
	else {
		return 0;
	}
}

void admin_record_set_custody_signal(struct ADMIN_RECORD* admin_record_ptr)
{
	admin_record_set_type(admin_record_ptr, 
		ADMIN_RECORD_TYPE_CUSTODY_SIGNAL);
}

int admin_record_is_reserved(struct ADMIN_RECORD* admin_record_ptr)
{
	if (admin_record_get_type(admin_record_ptr) != 
		ADMIN_RECORD_TYPE_STATUS_REPORT &&
		admin_record_get_type(admin_record_ptr) != 
		ADMIN_RECORD_TYPE_CUSTODY_SIGNAL){
		return 1;
	}
	else {
		return 0;
	}
}

void admin_record_set_reserved(struct ADMIN_RECORD* admin_record_ptr)
{
	admin_record_set_type(admin_record_ptr, 
		ADMIN_RECORD_TYPE_RESERVED);
}

int custody_signal_is_succeeded_flag(
	struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	return (custody_signal_ptr->status & 0x80) >> 7;
}
	

void custody_signal_set_succeeded_flag(
	struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status |= 0x80;
}

void custody_signal_clr_succeeded_flag(
	struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status &= ~0x80;
}

int custody_signal_is_no_addition(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	return (custody_signal_ptr->status & 0x7F) == 
		CUSTODY_SIGNAL_REASON_CODE_NO_ADDITION; 
}

void custody_signal_set_no_addition(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status &= 0x80;
	custody_signal_ptr->status |= CUSTODY_SIGNAL_REASON_CODE_NO_ADDITION;
}

int custody_signal_is_reserved(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	if (custody_signal_ptr->status != 
		CUSTODY_SIGNAL_REASON_CODE_NO_ADDITION &&
		custody_signal_ptr->status != 
		CUSTODY_SIGNAL_REASON_CODE_REDUNDANT &&
		custody_signal_ptr->status != 
		CUSTODY_SIGNAL_REASON_CODE_DEPLETED &&
		custody_signal_ptr->status != 
		CUSTODY_SIGNAL_REASON_CODE_DESTINATION_UNINTELLIGIBLE &&
		custody_signal_ptr->status != 
		CUSTODY_SIGNAL_REASON_CODE_NO_ROUTE &&
		custody_signal_ptr->status != 
		CUSTODY_SIGNAL_REASON_CODE_NO_CONTACT &&
		custody_signal_ptr->status != 
		CUSTODY_SIGNAL_REASON_CODE_BLOCK_UNINTELLIGIBLE){
		return 1;
	}
	else {
		return 0;
	}
}

void custody_signal_set_reserved(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status &= 0x80;
	custody_signal_ptr->status |= CUSTODY_SIGNAL_REASON_CODE_RESERVED;
}

int custody_signal_is_redundant(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	return (custody_signal_ptr->status & 0x7F) == 
		CUSTODY_SIGNAL_REASON_CODE_REDUNDANT; 
}

void custody_signal_set_redundant(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status &= 0x80;
	custody_signal_ptr->status |= CUSTODY_SIGNAL_REASON_CODE_REDUNDANT;
}

int custody_signal_is_depleted(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	return (custody_signal_ptr->status & 0x7F) == 
		CUSTODY_SIGNAL_REASON_CODE_DEPLETED; 
}

void custody_signal_set_depleted(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status &= 0x80;
	custody_signal_ptr->status |= CUSTODY_SIGNAL_REASON_CODE_DEPLETED;
}

int custody_signal_is_destination_unintelligible(
	struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	return (custody_signal_ptr->status & 0x7F) == 
		CUSTODY_SIGNAL_REASON_CODE_DESTINATION_UNINTELLIGIBLE; 
}

void custody_signal_set_destination_unintelligible(
	struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status &= 0x80;
	custody_signal_ptr->status |= 
		CUSTODY_SIGNAL_REASON_CODE_DESTINATION_UNINTELLIGIBLE;
}

int custody_signal_is_no_route(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	return (custody_signal_ptr->status & 0x7F) == 
		CUSTODY_SIGNAL_REASON_CODE_NO_ROUTE; 
}

void custody_signal_set_no_route(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status &= 0x80;
	custody_signal_ptr->status |= CUSTODY_SIGNAL_REASON_CODE_NO_ROUTE;
}

int custody_signal_is_no_contact(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	return (custody_signal_ptr->status & 0x7F) == 
		CUSTODY_SIGNAL_REASON_CODE_NO_CONTACT; 
}

void custody_signal_set_no_contact(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status &= 0x80;
	custody_signal_ptr->status |= CUSTODY_SIGNAL_REASON_CODE_NO_CONTACT;
}

int custody_signal_is_block_unintelligible(
	struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	return (custody_signal_ptr->status & 0x7F) == 
		CUSTODY_SIGNAL_REASON_CODE_BLOCK_UNINTELLIGIBLE; 
}

void custody_signal_set_block_unintelligible(
	struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	custody_signal_ptr->status &= 0x80;
	custody_signal_ptr->status |= 
		CUSTODY_SIGNAL_REASON_CODE_BLOCK_UNINTELLIGIBLE;
}

void custody_signal_encode(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	int offset;
	int tmp;
	char* payload;

	offset = 0;
	payload = custody_signal_ptr->payload;
	
	payload[offset] = custody_signal_ptr->type_and_flag;
	offset += 1;

	payload[offset] = custody_signal_ptr->status;
	offset += 1;

	tmp = sdnv_encode(payload + offset, 
		&custody_signal_ptr->time_of_signal_sec, 4);
	offset += tmp;

	tmp = sdnv_encode(payload + offset, 
		&custody_signal_ptr->time_of_signal_nsec, 4);
	offset += tmp;

	tmp = sdnv_encode(payload + offset, 
		&custody_signal_ptr->creation_time, 4);
	offset += tmp;

	tmp = sdnv_encode(payload + offset, 
		&custody_signal_ptr->creation_sequence_number, 4);
	offset += tmp;

	// no '//' between scheme and ssp
	custody_signal_ptr->src_endpoint_id_len = 
		strlen(custody_signal_ptr->src_endpoint_id.scheme) +
		strlen(custody_signal_ptr->src_endpoint_id.ssp) + 2;
	
	tmp = sdnv_encode(payload + offset,
		&custody_signal_ptr->src_endpoint_id_len, 4);
	offset += tmp;

	strcpy(payload + offset, custody_signal_ptr->src_endpoint_id.scheme);
	strcat(payload + offset, ":");
	strcat(payload + offset, custody_signal_ptr->src_endpoint_id.ssp);
	offset += custody_signal_ptr->src_endpoint_id_len;

	custody_signal_ptr->payload_len = offset;
}

void custody_signal_decode(struct CUSTODY_SIGNAL* custody_signal_ptr)
{
	int offset;
	int tmp;
	char* payload;
	char src_endpoint_id[BP_EID_MAXLEN];

	offset = 0;
	payload = custody_signal_ptr->payload;

	custody_signal_ptr->type_and_flag = payload[offset];
	offset += 1;

	custody_signal_ptr->status = payload[offset];
	offset += 1;

	sdnv_decode(&custody_signal_ptr->time_of_signal_sec,
		payload + offset, &tmp);
	offset += tmp;

	sdnv_decode(&custody_signal_ptr->time_of_signal_nsec,
		payload + offset, &tmp);
	offset += tmp;

	sdnv_decode(&custody_signal_ptr->creation_time,
		payload + offset, &tmp);
	offset += tmp;

	sdnv_decode(&custody_signal_ptr->creation_sequence_number,
		payload + offset, &tmp);
	offset += tmp;

	sdnv_decode(&custody_signal_ptr->src_endpoint_id_len,
		payload + offset, &tmp);
	offset += tmp;

	strcpy(src_endpoint_id, payload + offset);
	strcpy(custody_signal_ptr->src_endpoint_id.scheme, 
		strtok(src_endpoint_id, ":"));
	strcpy(custody_signal_ptr->src_endpoint_id.ssp, 
		strtok(NULL, ":"));
	
	tmp = strlen(payload + offset) + 1;
	offset += tmp;
	
	custody_signal_ptr->payload_len = offset;
}

int sdnv_encode(void* _to, void* _from, int from_len)
{
	int to_len;
	int from_bits;
	int to_sigbits, to_lead0bits;
	int i,j;
	unsigned char* to;
	unsigned char* from;
	int to_byte, from_byte, to_bit, from_bit;

	to = _to;
	from = _from;
	from_bits = 8*from_len;
	to_len = (from_bits+6)/7;

	to_sigbits = 7*to_len; 
	to_lead0bits = to_sigbits - from_bits;

	for (i = 0; i < to_len-1; i++){
		set_bit(to[i], 0);
	}
	clr_bit(to[to_len-1], 0);

	for (i = 0; i < to_lead0bits; i++){
		clr_bit(to[0], i+1);
	}

	// printf("indicated bits init ended!\n");
	// printf("to_len = %d\n", to_len);
	// printf("to_sigbits = %d, to_lead0bits = %d\n",
	// 	to_sigbits, to_lead0bits);

	// printf("to is:\n");
        // for (i = 0; i < 8; i++){
        //         printf("%02X ", to[i]);
        // }
        // printf("\n");

	to_byte = 0;
	for (i = 0; i < from_bits; i++){
		from_byte = from_len-1 - i/8;
		from_bit = i%8;
		to_byte = (to_lead0bits+i)/7;
		to_bit = ((to_lead0bits) + i)%7 + 1;
		// printf("from_byte = %d, from_bit = %d, "
		// 	"to_byte = %d, to_bit = %d ",
		// 	from_byte, from_bit, to_byte, to_bit);
		j = get_bit(from[from_byte], from_bit);
		ass_bit(to[to_byte], to_bit, j);
		// printf("j = %d\n", j);
	}

	return to_len;
}

int sdnv_decode(void* _to, void* _from, int* from_len_ptr)
{
	unsigned char* to;
	unsigned char* from;
	int from_len;
	int from_sigbits;
	int to_len;
	int from_lead0bits, to_lead0bits;
	int from_minuslead0bits;
	int i,j;
	int from_byte, from_bit, to_byte, to_bit;
	
	to = _to;
	from = _from;
	from_len = 1;
	while (get_bit(*from, 0) == 1){
		from_len++;
		from++;
	}
	if (from_len_ptr != NULL){
		*from_len_ptr = from_len;
	}

	from = _from;
	from_sigbits = 7*from_len;
	from_lead0bits = 0;
	for (i = 0; i < from_sigbits; i++){
		from_byte = i/7;
		from_bit = i%7 + 1;
		if (get_bit(from[from_byte], from_bit) == 1){
			break;
		}
		from_lead0bits++;
	}
	from_minuslead0bits = from_sigbits - from_lead0bits;

	/* not bit to put */
	if (from_minuslead0bits == 0){
		to_len = 1;
		*to = 0;
		return to_len;
	}

	/* length without leading zeroes */
	to_len = (from_minuslead0bits+7)/8;

	/* flush the most significant byte on to */
	if (to_len > 0){
		to[to_len-1] = 0;
	}

	/* calculate to leading zero bits by from's minuslead0bits */
	to_lead0bits = (8 - from_minuslead0bits%8) % 8;
	
	// printf("from_len = %d\n", from_len);
	// printf("to_len = %d, to_lead0bits = %d, from_sigbits = %d "
	// 	"from_lead0bits = %d\n",
	// 	to_len, to_lead0bits, from_sigbits, from_lead0bits);
	// printf("from_minuslead0bits = %d\n", from_minuslead0bits);

	from_byte = 0;
	for (i = 0; i < from_minuslead0bits; i++){
		from_byte = (from_lead0bits + i)/7;
		from_bit = ((from_lead0bits)+i)%7 + 1;
		to_byte = (to_lead0bits+i)/8;
		to_bit = (to_lead0bits+i)%8;
		// printf("from_byte = %d, from_bit = %d, "
		// 	"to_byte = %d, to_bit = %d ",
		// 	from_byte, from_bit, to_byte, to_bit);
		j = get_bit(from[from_byte], from_bit);
		ass_bit(to[to_byte], to_bit, j);
		// printf("j = %d\n", j);
	}
	// printf("sdnv from_byte = %d\n", from_byte);
	
	return to_len;
}

