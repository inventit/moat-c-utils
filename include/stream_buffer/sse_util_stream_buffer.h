/*
 * LEGAL NOTICE
 *
 * Copyright (C) 2015 InventIt Inc. All rights reserved.
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

#ifndef __SSE_UTIL_STREAM_BUFFER_H__
#define __SSE_UTIL_STREAM_BUFFER_H__

SSE_BEGIN_C_DECLS

/**
 * @brief Class for buffered read
 */
struct TSseUtilStreamBuffer_ {
  sse_char* fBuffer;    /** Buffer for read */
  sse_char* fBufferPos; /** Position next data should be stored */
  sse_uint fBufferSize; /** Size of fBuffer */
};
typedef struct TSseUtilStreamBuffer_ TSseUtilStreamBuffer;

/**
 * @brief Create StreamBuffer instance
 *
 * Create a StreamBuffer instance. StramBuffer is helper class to read data
 * from file descripter asynchronously.
 *
 * @param [in] in_size Size of StreamBuffer
 *
 * @retval != NULL  Address of StreamBuffer
 * @retval == NULL  Failure
 *
 */
TSseUtilStreamBuffer*
SseUtilStreamBuffer_New(sse_uint in_size);

/**
 * @brief Delete StreamBuffer instance
 * @ingroup StreamBuffer
 *
 * Delete StreamBuffer instance
 *
 * @param [in] self this pointer
 *
 * @return None
 */
void
TSseUtilStreamBuffer_Delete(TSseUtilStreamBuffer* self);

/**
 * @brief Initialize StreamBuffer
 *
 * If you allocate a memory for StreamBuffer on stack area or as class/struct member
 * without SseUtilStreamBuffer_New(), kindly initialize it with this function.
 *
 * @param [in] self this pointer
 * @param [in] in_size buffer size
 *
 * @retval SSE_E_OK Success
 * @retval others   Failuer
 */
sse_int
TSseUtilStreamBuffer_Initialize(TSseUtilStreamBuffer* self,
				sse_uint in_size);

/**
 * @brief Fnazlie StreamBuffer
 *
 * If you allocate a memory for StreamBuffer, please cleanup the instance
 * with this function.
 *
 * @param [in] self this pointer
 *
 * @retval SSE_E_OK Success
 * @retval others   Failure
 */
void
TSseUtilStreamBuffer_Finalize(TSseUtilStreamBuffer* self);

/**
 * @brief Read data from file descripter
 *
 * Please read date from file descripter in a callback of Moat I/O Watcher callback.
 *
 * @param [in] self this pointer
 * @param [in] in_desc file descripter
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
TSseUtilStreamBuffer_OnReadable(TSseUtilStreamBuffer* self,
				sse_int in_desc);

/**
 * @brief Read data from internal buffer of StermBuffer
 *
 * This function read date from internal buffer of StreamBuffer
 * stored by TSseUtilStreamBuffer_OnReadable().
 *
 * @param [in]  self This pointer
 * @param [out] out_buffer Buffer for read
 * @param [in]  in_buffer_size Size of out_buffer
 * @param [out] out_data_len Size of read data
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
TSseUtilStreamBuffer_Read(TSseUtilStreamBuffer* self,
			  sse_char* out_buffer,
			  sse_uint in_buffer_size,
			  sse_uint* out_data_len);

/**
 * @brief Read data from internal buffer of StermBuffer
 *
 * This function read date from internal buffer of StreamBuffer
 * stored by TSseUtilStreamBuffer_OnReadable().
 *
 * @param [in]  self This pointer
 * @param [out] out_buffer Buffer for read
 * @param [in]  in_buffer_size Size of out_buffer
 * @param [out] out_data_len Size of read data
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
TSseUtilStreamBuffer_Read(TSseUtilStreamBuffer* self,
			  sse_char* out_buffer,
			  sse_uint in_buffer_size,
			  sse_uint* out_data_len);

/**
 * @brief Read line from internal buffer of StermBuffer
 *
 * This function read lien separeted by CRLF or NULL character  from
 * internal buffer of StreamBuffer stored by TSseUtilStreamBuffer_OnReadable().
 * This function allocates a memory block for read line and return the address
 * with out_buffer. So caller size need to release the buffer by sse_free().
 *
 * @param [in]  self This pointer
 * @param [out] out_buffer Buffer for read
 * @param [in]  in_strip_crlf Flag for strip CRLF
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
TSseUtilStreamBuffer_ReadLine(TSseUtilStreamBuffer* self,
			      sse_char** out_buffer,
			      sse_bool in_strip_crlf);

/**
 * @brief Read line from internal buffer of StermBuffer
 *
 * This function read lien as same as TSseUtilStreamBuffer_ReadLine().
 * The difference is using SSEString for reading buffer.
 * This function allocates a SSEString instance and return the address
 * with out_buffer. So caller size need to release the buffer by sse_free().
 *
 * @param [in]  self This pointer
 * @param [out] out_buffer Buffer for read
 * @param [in]  in_strip_crlf Flag for strip CRLF
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
TSseUtilsStramBuffer_ReadListWithSSEString(TSseUtilStreamBuffer* self,
					   SSEString** out_string,
					   sse_bool in_strip_crlf);

/**
 * @brief Check any data in the buffer or not
 *
 * Check any data in the buffer on not.
 *
 * @param [in]  self This pointer
 *
 * @retval sse_true  There is a data in the buffer.
 * @retval sse_false No data in the buffer.
 */
sse_bool
TSseUtilsStramBuffer_HasData(TSseUtilStreamBuffer* self);

SSE_END_C_DECLS

#endif // __SSE_UTIL_STREAM_BUFFER_H__
