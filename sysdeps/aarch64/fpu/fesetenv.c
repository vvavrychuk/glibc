/* Copyright (C) 1997-2014 Free Software Foundation, Inc.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <fenv.h>
#include <fpu_control.h>

int
fesetenv (const fenv_t *envp)
{
  fpu_control_t fpcr;
  fpu_control_t fpcr_new;
  fpu_control_t updated_fpcr;
  fpu_fpsr_t fpsr;
  fpu_fpsr_t fpsr_new;

  _FPU_GETCW (fpcr);
  _FPU_GETFPSR (fpsr);

  fpcr_new = fpcr & _FPU_RESERVED;
  fpsr_new = fpsr & _FPU_FPSR_RESERVED;

  if (envp == FE_DFL_ENV)
    {
      fpcr_new |= _FPU_DEFAULT;
      fpsr_new |= _FPU_FPSR_DEFAULT;
    }
  else if (envp == FE_NOMASK_ENV)
    {
      fpcr_new |= _FPU_FPCR_IEEE;
      fpsr_new |= _FPU_FPSR_IEEE;
    }
  else
    {
      fpcr_new |= envp->__fpcr & ~_FPU_RESERVED;
      fpsr_new |= envp->__fpsr & ~_FPU_FPSR_RESERVED;
    }

  if (fpsr != fpsr_new)
    _FPU_SETFPSR (fpsr_new);

  if (fpcr != fpcr_new)
    _FPU_SETCW (fpcr_new);

  /* Trapping exceptions are optional in AArch64 the relevant enable
     bits in FPCR are RES0 hence the absence of support can be
     detected by reading back the FPCR and comparing with the required
     value.  */

  _FPU_GETCW (updated_fpcr);
  if ((updated_fpcr & fpcr_new) != fpcr_new)
    return 1;

  return 0;
}
libm_hidden_def (fesetenv)
