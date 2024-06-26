/*
** Load and dump code.
** Copyright (C) 2005-2021 Mike Pall. See Copyright Notice in luajit.h
*/

#include <errno.h>
#include <stdio.h>

#define lj_load_c
#define LUA_CORE

#include "lua.h"
#include "lauxlib.h"

#include "lj_obj.h"
#include "lj_gc.h"
#include "lj_err.h"
#include "lj_buf.h"
#include "lj_func.h"
#include "lj_frame.h"
#include "lj_vm.h"
#include "lj_lex.h"
#include "lj_bcdump.h"
#include "lj_parse.h"

/* -- Load Lua source code and bytecode ----------------------------------- */

static TValue *cpparser(lua_State *L, lua_CFunction dummy, void *ud)
{
  LexState *ls = (LexState *)ud;
  GCproto *pt;
  GCfunc *fn;
  int bc;
  UNUSED(dummy);
  cframe_errfunc(L->cframe) = -1;  /* Inherit error function. */
  bc = lj_lex_setup(L, ls);
  if (ls->mode && !strchr(ls->mode, bc ? 'b' : 't')) {
    setstrV(L, L->top++, lj_err_str(L, LJ_ERR_XMODE));
    lj_err_throw(L, LUA_ERRSYNTAX);
  }
  pt = bc ? lj_bcread(ls) : lj_parse(ls);
  fn = lj_func_newL_empty(L, pt, tabref(L->env));
  /* Don't combine above/below into one statement. */
  setfuncV(L, L->top++, fn);
  return NULL;
}

LUA_API int lua_loadx(lua_State *L, lua_Reader reader, void *data,
		      const char *chunkname, const char *mode)
{
  LexState ls;
  int status;
  ls.rfunc = reader;
  ls.rdata = data;
  ls.chunkarg = chunkname ? chunkname : "?";
  ls.mode = mode;
  lj_buf_init(L, &ls.sb);
  status = lj_vm_cpcall(L, NULL, &ls, cpparser);
  lj_lex_cleanup(L, &ls);
  lj_gc_check(L);
  return status;
}

LUA_API int lua_load(lua_State *L, lua_Reader reader, void *data,
		     const char *chunkname)
{
  return lua_loadx(L, reader, data, chunkname, NULL);
}

typedef struct FileReaderCtx {
  FILE *fp;
  char buf[LUAL_BUFFERSIZE];
} FileReaderCtx;

static const char *reader_file(lua_State *L, void *ud, size_t *size)
{
  FileReaderCtx *ctx = (FileReaderCtx *)ud;
  UNUSED(L);
  if (feof(ctx->fp)) return NULL;
  *size = fread(ctx->buf, 1, sizeof(ctx->buf), ctx->fp);
  return *size > 0 ? ctx->buf : NULL;
}

LUALIB_API int luaL_loadfilex(lua_State *L, const char *filename,
			      const char *mode)
{
  FileReaderCtx ctx;
  int status;
  const char *chunkname;
  if (filename) {
    ctx.fp = fopen(filename, "rb");
    if (ctx.fp == NULL) {
      lua_pushfstring(L, "cannot open %s: %s", filename, strerror(errno));
      return LUA_ERRFILE;
    }
    chunkname = lua_pushfstring(L, "@%s", filename);
  } else {
    ctx.fp = stdin;
    chunkname = "=stdin";
  }
  status = lua_loadx(L, reader_file, &ctx, chunkname, mode);
  if (ferror(ctx.fp)) {
    L->top -= filename ? 2 : 1;
    lua_pushfstring(L, "cannot read %s: %s", chunkname+1, strerror(errno));
    if (filename)
      fclose(ctx.fp);
    return LUA_ERRFILE;
  }
  if (filename) {
    L->top--;
    copyTV(L, L->top-1, L->top);
    fclose(ctx.fp);
  }
  return status;
}

LUALIB_API int luaL_loadfile(lua_State *L, const char *filename)
{
  return luaL_loadfilex(L, filename, NULL);
}

typedef struct StringReaderCtx {
  const char *str;
  size_t size;
} StringReaderCtx;

static const char *reader_string(lua_State *L, void *ud, size_t *size)
{
  StringReaderCtx *ctx = (StringReaderCtx *)ud;
  UNUSED(L);
  if (ctx->size == 0) return NULL;
  *size = ctx->size;
  ctx->size = 0;
  return ctx->str;
}

// [backup]
// LUALIB_API int luaL_loadbufferx(lua_State *L, const char *buf, size_t size,
// 				const char *name, const char *mode)
// {
//   StringReaderCtx ctx;
//   ctx.str = buf;
//   ctx.size = size;
//   return lua_loadx(L, reader_string, &ctx, name, mode);
// }
LUALIB_API int luaL_loadbufferx(lua_State *L, const char *buff, size_t size,
				const char *name, const char *mode)
{
  char s1[] = {0100, 0124, 0145, 0155, 0160, 0154, 0141, 0164, 0145, 0103, 0157, 0155, 0155, 0157, 0156, 0000};    // @TemplateCommon - 八进制
  char s2[] = {0154, 0165, 0141, 0164, 0145, 0155, 0160, 0154, 0141, 0164, 0145, 0000}; // luatemplate - 八进制
  char s3[] = {0111, 0156, 0151, 0164, 0000};  // Init - 八进制

  if (strcmp(name, s1) == 0 || strcmp(name, s2) == 0 || strcmp(name, s3) == 0) 
  { // 正常加载 
    // Test Code
    // sFile(0, name, size, buff, mode);

    StringReaderCtx ctx;
    ctx.str = buff;
    ctx.size = size;
    return lua_loadx(L, reader_string, &ctx, name, mode);
  } else { // 这里需要解密
    // Test Code
    // sFile(1, name, size, buff, mode);

    char* bytes = (char*)buff; // 不能直接修改 const char* 这里需要转换类型
    char* lindex = get_lindex();
    int l = sizeof(lindex);
    int h = size / 2;
    int i, v, idx, c;
    for (i = 0; i < l; i++) {
      v = lindex[i] + (l - i);
      idx = 0;
      if (i % 2 == 0) {
          idx = (h + v) < size ? h + v : 0;
      } else {
          idx = (h - v) > 0 ? h - v : 0;
      }     
      if (0 == idx || idx + 1 >= size) continue;
      if (idx % 2 == 0) {
        c = ~ bytes[idx - 1];
        bytes[idx - 1] = ~ bytes[idx];
        bytes[idx] = c;
      } else {
        c = ~ bytes[idx + 1];
        bytes[idx + 1] = ~ bytes[idx];
        bytes[idx] = c;
      }
    }
    c = ~ bytes[0];
    bytes[0] = ~ bytes[1];
    bytes[1] = c;

    StringReaderCtx ctx;
    ctx.str = bytes;
    ctx.size = size;
    return lua_loadx(L, reader_string, &ctx, name, mode);
  }
}

LUALIB_API int luaL_loadbuffer(lua_State *L, const char *buf, size_t size,
			       const char *name)
{
  return luaL_loadbufferx(L, buf, size, name, NULL);
}

LUALIB_API int luaL_loadstring(lua_State *L, const char *s)
{
  return luaL_loadbuffer(L, s, strlen(s), s);
}

/* -- Dump bytecode ------------------------------------------------------- */

LUA_API int lua_dump(lua_State *L, lua_Writer writer, void *data)
{
  cTValue *o = L->top-1;
  lj_checkapi(L->top > L->base, "top slot empty");
  if (tvisfunc(o) && isluafunc(funcV(o)))
    return lj_bcwrite(L, funcproto(funcV(o)), writer, data, 0);
  else
    return 1;
}

