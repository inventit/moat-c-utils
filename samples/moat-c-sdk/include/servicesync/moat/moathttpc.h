/*
 * LEGAL NOTICE
 *
 * Copyright (C) 2013 InventIt Inc. All rights reserved.
 *
 * This source code, product and/or document is protected under licenses 
 * restricting its use, copying, distribution, and decompilation.
 * No part of this source code, product or document may be reproduced in
 * any form by any means without prior written authorization of InventIt Inc.
 * and its licensors, if any.
 *
 * InventIt Inc.
 * 9F KOJIMACHI CP BUILDING
 * 4-4-7 Kojimachi, Chiyoda-ku, Tokyo 102-0083
 * JAPAN
 * http://www.yourinventit.com/
 */


#ifndef MOATHTTPC_H_
#define MOATHTTPC_H_


SSE_BEGIN_C_DECLS

typedef struct HttpClient_ MoatHttpClient;
typedef struct HttpRequest_ MoatHttpRequest;
typedef struct HttpResponse_ MoatHttpResponse;

enum moat_httpc_method_type_ {
  MOAT_HTTP_METHOD_HEAD,
  MOAT_HTTP_METHOD_GET,
  MOAT_HTTP_METHOD_PUT,
  MOAT_HTTP_METHOD_DELETE,
  MOAT_HTTP_METHOD_POST,
  MOAT_HTTP_METHOD_CONNECT,
  MOAT_HTTP_METHODs
};

enum moat_httpc_state_ {
  MOAT_HTTP_STATE_DORMANT,
  MOAT_HTTP_STATE_CONNECTING,
  MOAT_HTTP_STATE_SENDING,
  MOAT_HTTP_STATE_SENT,
  MOAT_HTTP_STATE_RECEIVING,
  MOAT_HTTP_STATE_RECEIVED,
  MOAT_HTTP_STATE_ERROR,
  MOAT_HTTP_STATEs
};

enum moat_http_req_cont_enc_type_ {
  MOAT_HTTP_REQ_CONT_ENC_IDENTITY,
  MOAT_HTTP_REQ_CONT_ENC_GZIP,
  MOAT_HTTP_REQ_CONT_ENC_DEFLATE,
  MOAT_HTTP_REQ_CONT_ENCs
};

enum moat_httpc_options_ {
  MOAT_HTTP_OPT_CONN_KEEP_ALIVE = 1,   /* bool */
  MOAT_HTTP_OPT_ACCEPT_COMPRESSED,     /* bool */
  MOAT_HTTP_OPT_REC_CONT_ENC,          /* http_req_cont_enc_type */
  MOAT_HTTP_OPT_CONN_TIMEOUT,          /* uint (sec) */
  MOAT_HTTP_OPT_SEND_TIMEOUT,          /* uint (sec) */
  MOAT_HTTP_OPT_RECV_TIMEOUT,          /* uint (sec) */
  MOAT_HTTP_OPTs
};

MoatHttpClient * moat_httpc_new(void);
void moat_httpc_free(MoatHttpClient *self);
void moat_httpc_reset(MoatHttpClient *self);
MoatHttpRequest * moat_httpc_create_request(MoatHttpClient *self, sse_int in_method, sse_char *in_url, sse_size in_url_len);
sse_int moat_httpc_send_request(MoatHttpClient *self, MoatHttpRequest *in_req);
sse_int moat_httpc_do_send(MoatHttpClient *self, sse_bool *out_complete);
sse_int moat_httpc_recv_response(MoatHttpClient *self);
sse_int moat_httpc_do_recv(MoatHttpClient *self, sse_bool *out_complete);
sse_int moat_httpc_set_user_agent(MoatHttpClient *self, sse_char *in_user_agent);
sse_int moat_httpc_set_option(MoatHttpClient *self, sse_int in_opt, sse_pointer in_opt_val, sse_size in_val_len);
MoatHttpResponse * moat_httpc_get_response(MoatHttpClient *self);
sse_int moat_httpc_get_state(MoatHttpClient *self);
sse_int moat_httpc_set_download_file_path(MoatHttpClient *self, sse_char *in_file, sse_size in_path_len);

void moat_httpreq_free(MoatHttpRequest *self);
MoatUrl * moat_httpreq_get_url(MoatHttpRequest *self);
sse_int moat_httpreq_add_query(MoatHttpRequest *self, sse_char *in_key, sse_size in_key_len, sse_char *in_value, sse_size in_value_len);
sse_int moat_httpreq_add_header(MoatHttpRequest *self, sse_char *in_hdr, sse_size in_hdr_len, sse_char *in_cont, sse_size in_cont_len);
sse_int moat_httpreq_remove_header(MoatHttpRequest *self, sse_char *in_hdr, sse_size in_hdr_len);
sse_int moat_httpreq_set_data(MoatHttpRequest *self, sse_pointer in_data, sse_size in_data_len, sse_char *in_cont_type, sse_size in_type_len);
sse_int moat_httpreq_set_upload_file_path(MoatHttpRequest *self, sse_char *in_file, sse_size in_path_len, sse_char *in_cont_type, sse_size in_type_len);
sse_int moat_httpreq_get_host(MoatHttpRequest *self, sse_char **out_host, sse_size *out_len);
sse_int moat_httpreq_get_port(MoatHttpRequest *self, sse_uint16 *out_port);

void moat_httpres_free(MoatHttpResponse *self);
sse_int moat_httpres_get_status_code(MoatHttpResponse *self, sse_int *out_status_code);
sse_bool moat_httpres_need_redirect(MoatHttpResponse *self);
sse_int moat_httpres_peek_body(MoatHttpResponse *self, sse_byte **out_body, sse_size *out_body_len);
sse_int moat_httpres_get_header_value(MoatHttpResponse *self, sse_char *in_field, sse_size in_field_len, sse_char **out_value, sse_size *out_value_len);
sse_int moat_httpres_get_redirect_to(MoatHttpResponse *self, sse_char **out_url, sse_size *out_url_len);


typedef struct MoatXxxloader_ MoatDownloader;
typedef void (*MoatDownloader_NotifyCompletionProc)(MoatDownloader *in_dl, sse_bool in_canceled, sse_pointer in_user_data);
typedef void (*MoatDownloader_NotifyErrorProc)(MoatDownloader *in_dl, sse_int in_err_code, sse_pointer in_user_data);

MoatDownloader * moat_downloader_new(void);
void moat_downloader_free(MoatDownloader *self);
MoatHttpClient * moat_downloader_get_http_client(MoatDownloader *self);
void moat_downloader_set_callbacks(MoatDownloader *self, MoatDownloader_NotifyCompletionProc in_cproc, MoatDownloader_NotifyErrorProc in_eproc, sse_pointer in_user_data);
sse_int moat_downloader_download(MoatDownloader *self, sse_char *in_url, sse_size in_url_len, sse_char *in_file_path);
void moat_downloader_cancel_download(MoatDownloader *self);
void moat_downloader_reset(MoatDownloader *self);

typedef struct MoatXxxloader_ MoatUploader;
typedef void (*MoatUploader_NotifyCompletionProc)(MoatUploader *in_dl, sse_bool in_canceled, sse_pointer in_user_data);
typedef void (*MoatUploader_NotifyErrorProc)(MoatUploader *in_dl, sse_int in_err_code, sse_pointer in_user_data);

MoatUploader * moat_uploader_new(void);
void moat_uploader_free(MoatUploader *self);
MoatHttpClient * moat_uploader_get_http_client(MoatUploader *self);
void moat_uploader_set_callbacks(MoatDownloader *self, MoatUploader_NotifyCompletionProc in_cproc, MoatUploader_NotifyErrorProc in_eproc, sse_pointer in_user_data);
sse_int moat_uploader_upload(MoatDownloader *self, sse_bool in_is_post, sse_char *in_url, sse_size in_url_len, sse_char *in_file_path);
void moat_uploader_cancel_upload(MoatDownloader *self);
void moat_uploader_reset(MoatDownloader *self);

SSE_END_C_DECLS

#endif /* MOATHTTPC_H_ */
