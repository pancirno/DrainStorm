
/* ============================================================
   1000:0012  FUN_1000_0012
   ============================================================ */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1000_0012(void)

{
  code *pcVar1;
  undefined2 uVar2;
  int iVar3;
  uint in_CX;
  undefined2 in_BX;
  undefined2 unaff_SI;
  undefined2 unaff_DI;
  undefined2 unaff_ES;
  undefined2 unaff_CS;
  bool bVar4;
  undefined4 uVar5;
  undefined2 uVar6;
  
  do {
    do {
      FUN_1000_01f5(unaff_CS,0xff);
      unaff_CS = 0x11d0;
      uVar5 = INITTASK();
    } while ((int)uVar5 == 0);
    bVar4 = 0xfeff < in_CX;
    in_CX = in_CX + 0x100;
    DAT_1080_24b2 = unaff_ES;
  } while (bVar4);
  DAT_1080_247c = in_CX;
  DAT_1080_247e = unaff_SI;
  DAT_1080_2480 = unaff_DI;
  DAT_1080_2482 = in_BX;
  DAT_1080_2484 = unaff_ES;
  DAT_1080_2486 = (int)((ulong)uVar5 >> 0x10);
  LOCKSEGMENT(0x11d0,0xffff);
  uVar6 = 0x56;
  uVar2 = GETVERSION(0x11d0);
  DAT_1080_24b4 = CONCAT11((char)uVar2,(char)((uint)uVar2 >> 8));
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar1 = (code *)swi(0x21);
    DAT_1080_24b8 = (*pcVar1)();
  }
  else {
    uVar6 = 0x1000;
    DAT_1080_24b8 = DOS3CALL(0x1000);
  }
  _DAT_1080_24b6 = CONCAT11((char)DAT_1080_24b8,(char)((uint)DAT_1080_24b8 >> 8));
  if ((DAT_1000_0010 & 1) == 0) {
    DAT_1080_24bb = 0;
  }
  WAITEVENT(0x1000,0,uVar6);
  iVar3 = INITAPP(0x11d0,DAT_1080_2480);
  if (iVar3 == 0) {
    FUN_1000_0012();
    return;
  }
  FUN_1000_0104();
  FUN_1000_0346(0x1000);
  FUN_1000_04f0();
  FUN_1000_20c4();
  uVar2 = FUN_1000_00d8(DAT_1080_24ec,DAT_1080_24ee,DAT_1080_24f0,DAT_1080_24f2,DAT_1080_24f4);
  FUN_1000_01e7(0x1000,uVar2);
  FUN_1000_05b4();
  return;
}



/* ============================================================
   1000:001a  entry
   ============================================================ */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void entry(void)

{
  code *pcVar1;
  undefined2 uVar2;
  int iVar3;
  uint in_CX;
  undefined2 in_BX;
  undefined2 unaff_SI;
  undefined2 unaff_DI;
  undefined2 unaff_ES;
  undefined2 unaff_CS;
  bool bVar4;
  undefined4 uVar5;
  undefined2 uVar6;
  
  do {
    uVar5 = INITTASK(unaff_CS,0);
    if ((int)uVar5 != 0) {
      bVar4 = in_CX < 0xff00;
      in_CX = in_CX + 0x100;
      DAT_1080_24b2 = unaff_ES;
      if (bVar4) {
        DAT_1080_247c = in_CX;
        DAT_1080_247e = unaff_SI;
        DAT_1080_2480 = unaff_DI;
        DAT_1080_2482 = in_BX;
        DAT_1080_2484 = unaff_ES;
        DAT_1080_2486 = (int)((ulong)uVar5 >> 0x10);
        LOCKSEGMENT(0x11d0,0xffff);
        uVar6 = 0x56;
        uVar2 = GETVERSION(0x11d0);
        DAT_1080_24b4 = CONCAT11((char)uVar2,(char)((uint)uVar2 >> 8));
        if ((DAT_1000_0010 & 1) == 0) {
          pcVar1 = (code *)swi(0x21);
          DAT_1080_24b8 = (*pcVar1)();
        }
        else {
          uVar6 = 0x1000;
          DAT_1080_24b8 = DOS3CALL(0x1000);
        }
        _DAT_1080_24b6 = CONCAT11((char)DAT_1080_24b8,(char)((uint)DAT_1080_24b8 >> 8));
        if ((DAT_1000_0010 & 1) == 0) {
          DAT_1080_24bb = 0;
        }
        WAITEVENT(0x1000,0,uVar6);
        iVar3 = INITAPP(0x11d0,DAT_1080_2480);
        if (iVar3 == 0) {
          FUN_1000_0012();
          return;
        }
        FUN_1000_0104();
        FUN_1000_0346(0x1000);
        FUN_1000_04f0();
        FUN_1000_20c4();
        uVar2 = FUN_1000_00d8(DAT_1080_24ec,DAT_1080_24ee,DAT_1080_24f0,DAT_1080_24f2,DAT_1080_24f4)
        ;
        FUN_1000_01e7(0x1000,uVar2);
        FUN_1000_05b4();
        return;
      }
    }
    unaff_CS = 0x1000;
    FUN_1000_01f5();
  } while( true );
}



/* ============================================================
   1000:00d8  FUN_1000_00d8
   ============================================================ */

void __cdecl16far FUN_1000_00d8(void)

{
  FUN_1008_0000(DAT_1080_2486,DAT_1080_2482,DAT_1080_2484,DAT_1080_247e,DAT_1080_2480);
  return;
}



/* ============================================================
   1000:0104  FUN_1000_0104
   ============================================================ */

void __cdecl16far FUN_1000_0104(void)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  int iVar4;
  code *pcVar5;
  int iVar6;
  undefined2 in_BX;
  int unaff_BP;
  byte *pbVar7;
  byte *pbVar8;
  undefined2 unaff_ES;
  undefined2 uVar9;
  undefined2 uVar10;
  bool bVar11;
  
  uVar9 = 0x1000;
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar5 = (code *)swi(0x21);
    (*pcVar5)(unaff_BP + 1);
  }
  else {
    uVar9 = 0x11d0;
    DOS3CALL(0x1000,0x1080);
  }
  uVar10 = 0x1000;
  DAT_1080_249e._0_2_ = in_BX;
  DAT_1080_249e._2_2_ = unaff_ES;
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar5 = (code *)swi(0x21);
    (*pcVar5)();
  }
  else {
    uVar9 = 0x1000;
    uVar10 = 0x11d0;
    DOS3CALL(0x1000);
  }
  uVar9 = FUN_1000_06f6(uVar9);
  if (*(int *)&DAT_1080_2618 != 0) {
    bVar11 = false;
    (*(code *)*(undefined2 *)&DAT_1080_2616)(uVar10);
    if (bVar11) {
      FUN_1000_02e6();
      return;
    }
    (*(code *)*(undefined2 *)&DAT_1080_2616)(uVar10);
  }
  iVar4 = *(int *)0x2c;
  if (iVar4 != 0) {
    pbVar8 = (byte *)0x0;
    do {
      if (*pbVar8 == 0) break;
      iVar6 = 0xd;
      pbVar7 = (byte *)s__C_FILE_INFO__1080_2490;
      bVar11 = false;
      do {
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        pbVar3 = pbVar8;
        pbVar8 = pbVar8 + 1;
        pbVar1 = pbVar7;
        pbVar7 = pbVar7 + 1;
        bVar11 = *pbVar1 == *pbVar3;
      } while (bVar11);
      if (bVar11) {
        pbVar7 = (byte *)0x24c4;
        goto LAB_1000_01af;
      }
      iVar6 = 0x7fff;
      bVar11 = true;
      do {
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        pbVar1 = pbVar8;
        pbVar8 = pbVar8 + 1;
        bVar11 = *pbVar1 == 0;
      } while (!bVar11);
    } while (bVar11);
  }
LAB_1000_01c3:
  FUN_1000_02ae();
  FUN_1000_02ae();
  FUN_1000_02ae();
  return;
LAB_1000_01af:
  pbVar1 = pbVar8;
  pbVar2 = pbVar8 + 1;
  if (*pbVar1 < 0x41) goto LAB_1000_01c3;
  pbVar8 = pbVar8 + 2;
  if (*pbVar2 < 0x41) goto LAB_1000_01c3;
  pbVar3 = pbVar7;
  pbVar7 = pbVar7 + 1;
  *pbVar3 = *pbVar2 + 0xbf | (*pbVar1 + 0xbf) * '\x10';
  goto LAB_1000_01af;
}



/* ============================================================
   1000:01e7  FUN_1000_01e7
   ============================================================ */

void FUN_1000_01e7(void)

{
  FUN_1000_0224();
  return;
}



/* ============================================================
   1000:01f5  FUN_1000_01f5
   ============================================================ */

void FUN_1000_01f5(void)

{
  FUN_1000_0224();
  return;
}



/* ============================================================
   1000:0224  FUN_1000_0224
   ============================================================ */

void __cdecl16far FUN_1000_0224(void)

{
  code *pcVar1;
  undefined2 in_CX;
  int unaff_BP;
  undefined2 unaff_SS;
  char cVar2;
  
  DAT_1080_24fd = (undefined1)((uint)in_CX >> 8);
  if ((char)in_CX == '\0') {
    FUN_1000_02ae();
    FUN_1000_02ae();
    FUN_1000_20c4(*(undefined2 *)(unaff_BP + 6));
  }
  cVar2 = (char)((uint)in_CX >> 8);
  FUN_1000_02ae();
  FUN_1000_02ae();
  FUN_1000_0285();
  if (cVar2 == '\0') {
    if ((DAT_1000_0010 & 1) == 0) {
      pcVar1 = (code *)swi(0x21);
      (*pcVar1)();
    }
    else {
      DOS3CALL(0x1000);
    }
  }
  return;
}



/* ============================================================
   1000:0285  FUN_1000_0285
   ============================================================ */

void __cdecl16near FUN_1000_0285(void)

{
  code *pcVar1;
  
  if (DAT_1080_2618 != 0) {
    (*DAT_1080_2616)();
  }
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
  }
  else {
    DOS3CALL(0x1000,0x1080);
  }
  return;
}



/* ============================================================
   1000:02ae  FUN_1000_02ae
   ============================================================ */

void __cdecl16near FUN_1000_02ae(void)

{
  int *piVar1;
  int *unaff_SI;
  int *unaff_DI;
  int *piVar2;
  
  while (unaff_SI < unaff_DI) {
    piVar2 = unaff_DI + -2;
    piVar1 = unaff_DI + -1;
    unaff_DI = piVar2;
    if (*piVar2 != 0 || *piVar1 != 0) {
      (*(code *)*piVar2)();
    }
  }
  return;
}



/* ============================================================
   1000:02c2  FUN_1000_02c2
   ============================================================ */

void __cdecl16far FUN_1000_02c2(void)

{
  FUN_1000_062d(0xfc);
  FUN_1000_062d(0xff);
  return;
}



/* ============================================================
   1000:02e6  FUN_1000_02e6
   ============================================================ */

void FUN_1000_02e6(void)

{
  FUN_1000_05b4();
  return;
}



/* ============================================================
   1000:030c  FUN_1000_030c
   ============================================================ */

/* WARNING (jumptable): Unable to track spacebase fully for stack */
/* WARNING: Unable to track spacebase fully for stack */

void __cdecl16far FUN_1000_030c(void)

{
  int in_AX;
  undefined1 *puVar1;
  uint uVar2;
  undefined2 uVar3;
  undefined2 unaff_SS;
  undefined2 in_stack_00000000;
  undefined2 in_stack_00000002;
  
  puVar1 = (undefined1 *)(in_AX + 1U & 0xfffe);
  if ((puVar1 < &stack0x0004) && (uVar2 = -((int)puVar1 - (int)&stack0x0004), *(uint *)0xa <= uVar2)
     ) {
    if (uVar2 < *(uint *)0xc) {
      *(uint *)0xc = uVar2;
    }
    *(undefined2 *)(uVar2 - 2) = in_stack_00000002;
    *(undefined2 *)(uVar2 - 4) = in_stack_00000000;
    return;
  }
  uVar3 = FUN_1000_06f6();
  if (*(int *)0x2502 != -1) {
                    /* WARNING: Could not recover jumptable at 0x1000033c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(ulong)*(uint *)0x2502)();
    return;
  }
  FUN_1000_05b4();
  return;
}



/* ============================================================
   1000:0346  FUN_1000_0346
   ============================================================ */

/* WARNING (jumptable): Unable to track spacebase fully for stack */
/* WARNING: Unable to track spacebase fully for stack */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_1000_0346(int param_1)

{
  char *pcVar1;
  char cVar2;
  int iVar3;
  undefined2 uVar4;
  uint uVar5;
  uint uVar6;
  undefined2 *puVar7;
  char *pcVar8;
  char *pcVar9;
  int iVar10;
  char *pcVar11;
  undefined2 uVar12;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 in_stack_00000000;
  int aiStack_4 [2];
  
  DAT_1080_2508 = param_1;
  param_1 = 0x357;
  DAT_1080_2506 = in_stack_00000000;
  DAT_1080_24f6 = FUN_1000_066a();
  param_1 = (int)((ulong)DAT_1080_24f6 >> 0x10);
  uVar4 = (undefined2)DAT_1080_24f6;
  aiStack_4[1] = DAT_1080_2480;
  aiStack_4[0] = param_1;
  iVar3 = GETMODULEFILENAME(unaff_CS,0x104,uVar4);
  *(undefined1 *)(aiStack_4[1] + iVar3) = 0;
  iVar3 = 1;
  DAT_1080_24ec = 1;
  pcVar8 = (char *)s_1997_Dave_Horner_1080_007b + 6;
LAB_1000_0386:
  do {
    do {
      pcVar1 = pcVar8;
      pcVar8 = pcVar8 + 1;
      cVar2 = *pcVar1;
    } while (cVar2 == ' ');
  } while (cVar2 == '\t');
  if ((cVar2 != '\r') && (cVar2 != '\0')) {
    DAT_1080_24ec = DAT_1080_24ec + 1;
    do {
      pcVar8 = pcVar8 + -1;
LAB_1000_0399:
      pcVar1 = pcVar8;
      pcVar8 = pcVar8 + 1;
      cVar2 = *pcVar1;
      if ((cVar2 == ' ') || (cVar2 == '\t')) goto LAB_1000_0386;
      if ((cVar2 == '\r') || (cVar2 == '\0')) break;
      if (cVar2 == '\"') {
LAB_1000_03d2:
        do {
          while( true ) {
            while( true ) {
              pcVar1 = pcVar8;
              pcVar8 = pcVar8 + 1;
              cVar2 = *pcVar1;
              if ((cVar2 == '\r') || (cVar2 == '\0')) goto LAB_1000_0402;
              if (cVar2 == '\"') goto LAB_1000_0399;
              if (cVar2 == '\\') break;
              iVar3 = iVar3 + 1;
            }
            uVar5 = 0;
            do {
              pcVar11 = pcVar8;
              uVar5 = uVar5 + 1;
              pcVar8 = pcVar11 + 1;
            } while (*pcVar11 == '\\');
            if (*pcVar11 == '\"') break;
            iVar3 = iVar3 + uVar5;
            pcVar8 = pcVar11;
          }
          iVar3 = iVar3 + (uVar5 >> 1) + (uint)((uVar5 & 1) != 0);
        } while ((uVar5 & 1) != 0);
        goto LAB_1000_0399;
      }
      if (cVar2 != '\\') {
        iVar3 = iVar3 + 1;
        goto LAB_1000_0399;
      }
      uVar5 = 0;
      do {
        uVar5 = uVar5 + 1;
        pcVar1 = pcVar8;
        pcVar8 = pcVar8 + 1;
      } while (*pcVar1 == '\\');
      if (*pcVar1 == '\"') {
        iVar3 = iVar3 + (uVar5 >> 1) + (uint)((uVar5 & 1) != 0);
        if ((uVar5 & 1) == 0) goto LAB_1000_03d2;
        goto LAB_1000_0399;
      }
      iVar3 = iVar3 + uVar5;
    } while( true );
  }
LAB_1000_0402:
  iVar10 = (DAT_1080_24ec + 1) * 4;
  iVar3 = -(iVar3 + DAT_1080_24ec + iVar10 + 1U & 0xfffe);
  DAT_1080_24ee = (undefined1 *)((int)&param_1 + iVar3);
  pcVar11 = (char *)((int)&param_1 + iVar10 + iVar3);
  *(undefined2 *)(&stack0x0000 + iVar3) = unaff_SS;
  uVar12 = *(undefined2 *)(&stack0x0000 + iVar3);
  *(undefined2 *)((int)&param_1 + iVar3) = (undefined2)DAT_1080_24f6;
  *(undefined2 *)(&stack0x0004 + iVar3) = DAT_1080_24f6._2_2_;
  puVar7 = (undefined2 *)(&stack0x0006 + iVar3);
  *(int *)((int)&stack0x0000 + iVar3) = (int)&param_1 + iVar3;
  *(undefined2 *)((int)aiStack_4 + iVar3 + 2) = 0x11d0;
  *(undefined2 *)((int)aiStack_4 + iVar3) = 0x439;
  DAT_1080_24f0 = unaff_SS;
  uVar4 = FUN_1000_06f6();
  uVar4 = *(undefined2 *)&DAT_1080_24b2;
  pcVar8 = (char *)s_1997_Dave_Horner_1080_007b + 6;
LAB_1000_0448:
  do {
    do {
      pcVar1 = pcVar8;
      pcVar8 = pcVar8 + 1;
      cVar2 = *pcVar1;
    } while (cVar2 == ' ');
  } while (cVar2 == '\t');
  if ((cVar2 == '\r') || (cVar2 == '\0')) {
LAB_1000_04db:
    *(undefined2 *)(&stack0x0000 + iVar3) = 0x11d0;
    *(undefined2 *)((int)aiStack_4 + iVar3 + 2) = 0x4df;
    uVar4 = FUN_1000_06f6();
    *puVar7 = 0;
    puVar7[1] = 0;
                    /* WARNING: Could not recover jumptable at 0x100004ec. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(ulong)*(uint *)&DAT_1080_2506)();
    return;
  }
  *puVar7 = pcVar11;
  puVar7[1] = unaff_SS;
  puVar7 = puVar7 + 2;
  do {
    pcVar8 = pcVar8 + -1;
LAB_1000_0469:
    pcVar1 = pcVar8;
    pcVar8 = pcVar8 + 1;
    cVar2 = *pcVar1;
    if ((cVar2 == ' ') || (cVar2 == '\t')) {
      pcVar1 = pcVar11;
      pcVar11 = pcVar11 + 1;
      *pcVar1 = '\0';
      goto LAB_1000_0448;
    }
    if ((cVar2 == '\r') || (cVar2 == '\0')) {
LAB_1000_04d8:
      *pcVar11 = '\0';
      goto LAB_1000_04db;
    }
    pcVar9 = pcVar8;
    if (cVar2 == '\"') {
LAB_1000_04a5:
      while( true ) {
        pcVar8 = pcVar9 + 1;
        cVar2 = *pcVar9;
        if ((cVar2 == '\r') || (cVar2 == '\0')) goto LAB_1000_04d8;
        if (cVar2 == '\"') break;
        if (cVar2 == '\\') {
          uVar5 = 0;
          do {
            pcVar9 = pcVar8;
            uVar5 = uVar5 + 1;
            pcVar8 = pcVar9 + 1;
          } while (*pcVar9 == '\\');
          if (*pcVar9 == '\"') {
            for (uVar6 = uVar5 >> 1; uVar6 != 0; uVar6 = uVar6 - 1) {
              pcVar1 = pcVar11;
              pcVar11 = pcVar11 + 1;
              *pcVar1 = '\\';
            }
            if ((uVar5 & 1) == 0) break;
            pcVar1 = pcVar11;
            pcVar11 = pcVar11 + 1;
            *pcVar1 = '\"';
            pcVar9 = pcVar8;
          }
          else {
            for (; uVar5 != 0; uVar5 = uVar5 - 1) {
              pcVar1 = pcVar11;
              pcVar11 = pcVar11 + 1;
              *pcVar1 = '\\';
            }
          }
        }
        else {
          pcVar1 = pcVar11;
          pcVar11 = pcVar11 + 1;
          *pcVar1 = cVar2;
          pcVar9 = pcVar8;
        }
      }
      goto LAB_1000_0469;
    }
    if (cVar2 != '\\') {
      pcVar1 = pcVar11;
      pcVar11 = pcVar11 + 1;
      *pcVar1 = cVar2;
      goto LAB_1000_0469;
    }
    uVar5 = 0;
    do {
      uVar5 = uVar5 + 1;
      pcVar1 = pcVar8;
      pcVar8 = pcVar8 + 1;
    } while (*pcVar1 == '\\');
    if (*pcVar1 == '\"') {
      for (uVar6 = uVar5 >> 1; uVar6 != 0; uVar6 = uVar6 - 1) {
        pcVar1 = pcVar11;
        pcVar11 = pcVar11 + 1;
        *pcVar1 = '\\';
      }
      pcVar9 = pcVar8;
      if ((uVar5 & 1) == 0) goto LAB_1000_04a5;
      pcVar1 = pcVar11;
      pcVar11 = pcVar11 + 1;
      *pcVar1 = '\"';
      goto LAB_1000_0469;
    }
    for (; uVar5 != 0; uVar5 = uVar5 - 1) {
      pcVar1 = pcVar11;
      pcVar11 = pcVar11 + 1;
      *pcVar1 = '\\';
    }
  } while( true );
}



/* ============================================================
   1000:04f0  FUN_1000_04f0
   ============================================================ */

void __cdecl16far FUN_1000_04f0(void)

{
  int *piVar1;
  int *piVar2;
  char cVar3;
  undefined2 *puVar4;
  int iVar5;
  int iVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  int iVar9;
  int *piVar10;
  int *piVar11;
  char *pcVar12;
  int *piVar13;
  bool bVar14;
  undefined4 uVar15;
  char *pcVar16;
  undefined2 *puVar17;
  
  uVar15 = GETDOSENVIRONMENT();
  iVar6 = (int)((ulong)uVar15 >> 0x10);
  if ((int)uVar15 != 0) {
    iVar6 = 0;
  }
  iVar9 = 0;
  pcVar12 = (char *)0x0;
  iVar5 = -1;
  if (iVar6 != 0) {
    cVar3 = *(char *)0x0;
    while (cVar3 != '\0') {
      do {
        if (iVar5 == 0) break;
        iVar5 = iVar5 + -1;
        pcVar16 = pcVar12;
        pcVar12 = pcVar12 + 1;
      } while (*pcVar16 != '\0');
      iVar9 = iVar9 + 1;
      pcVar16 = pcVar12;
      pcVar12 = pcVar12 + 1;
      cVar3 = *pcVar16;
    }
  }
  pcVar16 = (char *)FUN_1000_066a();
  uVar7 = (undefined2)((ulong)pcVar16 >> 0x10);
  pcVar12 = (char *)pcVar16;
  puVar17 = (undefined2 *)FUN_1000_066a();
  uVar8 = (undefined2)((ulong)puVar17 >> 0x10);
  DAT_1080_24f2 = (undefined2 *)puVar17;
  piVar10 = (int *)0x0;
  do {
    DAT_1080_24f4 = (undefined2)((ulong)puVar17 >> 0x10);
    puVar4 = (undefined2 *)puVar17;
    if (iVar9 == 0) {
      *puVar4 = 0;
      puVar4[1] = 0;
      return;
    }
    bVar14 = *piVar10 == s__C_FILE_INFO__1080_2490._0_2_;
    if (bVar14) {
      piVar13 = (int *)s__C_FILE_INFO__1080_2490;
      iVar5 = 6;
      piVar11 = piVar10;
      do {
        if (iVar5 == 0) break;
        iVar5 = iVar5 + -1;
        piVar2 = piVar13;
        piVar13 = piVar13 + 1;
        piVar1 = piVar11;
        piVar11 = piVar11 + 1;
        bVar14 = *piVar1 == *piVar2;
      } while (bVar14);
      if (!bVar14) goto LAB_1000_0581;
    }
    else {
LAB_1000_0581:
      *puVar4 = pcVar12;
      puVar4[1] = uVar7;
      puVar17 = (undefined2 *)CONCAT22(DAT_1080_24f4,puVar4 + 2);
    }
    do {
      DAT_1080_24f4 = (undefined2)((ulong)puVar17 >> 0x10);
      piVar1 = piVar10;
      piVar10 = (int *)((int)piVar10 + 1);
      iVar5 = *piVar1;
      pcVar16 = pcVar12;
      pcVar12 = pcVar12 + 1;
      *pcVar16 = (char)iVar5;
      puVar17 = (undefined2 *)CONCAT22(DAT_1080_24f4,(undefined2 *)puVar17);
    } while ((char)iVar5 != '\0');
    iVar9 = iVar9 + -1;
  } while( true );
}



/* ============================================================
   1000:05b4  FUN_1000_05b4
   ============================================================ */

int * FUN_1000_05b4(void)

{
  int *piVar1;
  char *pcVar2;
  undefined2 in_AX;
  char *pcVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  char *pcVar7;
  undefined2 unaff_CS;
  int in_stack_00000000;
  undefined2 uVar8;
  
  uVar8 = in_AX;
  FUN_1000_02c2();
  FUN_1000_062d(in_AX);
  pcVar3 = (char *)FUN_1000_05f6(uVar8);
  if (pcVar3 != (char *)0x0) {
    iVar4 = 9;
    if (*pcVar3 == 'M') {
      iVar4 = 0xf;
    }
    pcVar3 = pcVar3 + iVar4;
    iVar4 = 0x22;
    pcVar7 = pcVar3;
    do {
      if (iVar4 == 0) break;
      iVar4 = iVar4 + -1;
      pcVar2 = pcVar7;
      pcVar7 = pcVar7 + 1;
    } while (*pcVar2 != '\r');
    pcVar7[-1] = '\0';
  }
  uVar8 = 0;
  FATALAPPEXIT(unaff_CS,pcVar3,0x1080);
  FATALEXIT(0x11d0,0xff,uVar8);
  piVar5 = (int *)&DAT_1080_2816;
  do {
    piVar1 = piVar5;
    piVar5 = piVar5 + 1;
    piVar6 = piVar5;
    if ((*piVar1 == in_stack_00000000) || (piVar6 = (int *)0x0, *piVar1 == -1)) {
      return piVar6;
    }
    iVar4 = -1;
    do {
      if (iVar4 == 0) break;
      iVar4 = iVar4 + -1;
      piVar1 = piVar5;
      piVar5 = (int *)((int)piVar5 + 1);
    } while ((char)*piVar1 != '\0');
  } while( true );
}



/* ============================================================
   1000:05f6  FUN_1000_05f6
   ============================================================ */

int * __stdcall16far FUN_1000_05f6(int param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  
  piVar3 = (int *)&DAT_1080_2816;
  do {
    piVar1 = piVar3;
    piVar3 = piVar3 + 1;
    piVar4 = piVar3;
    if ((*piVar1 == param_1) || (piVar4 = (int *)0x0, *piVar1 == -1)) {
      return piVar4;
    }
    iVar2 = -1;
    do {
      if (iVar2 == 0) break;
      iVar2 = iVar2 + -1;
      piVar1 = piVar3;
      piVar3 = (int *)((int)piVar3 + 1);
    } while ((char)*piVar1 != '\0');
  } while( true );
}



/* ============================================================
   1000:062d  FUN_1000_062d
   ============================================================ */

void __stdcall16far FUN_1000_062d(undefined2 param_1)

{
  char *pcVar1;
  char *pcVar2;
  int iVar3;
  undefined2 unaff_ES;
  
  if (DAT_1080_2612 != 0) {
    pcVar2 = (char *)FUN_1000_05f6(param_1);
    if (pcVar2 != (char *)0x0) {
      iVar3 = -1;
      do {
        if (iVar3 == 0) break;
        iVar3 = iVar3 + -1;
        pcVar1 = pcVar2;
        pcVar2 = pcVar2 + 1;
      } while (*pcVar1 != '\0');
      FUN_1000_20c9();
    }
  }
  return;
}



/* ============================================================
   1000:066a  FUN_1000_066a
   ============================================================ */

void __cdecl16near FUN_1000_066a(void)

{
  undefined2 uVar1;
  int iVar2;
  int in_DX;
  
  uVar1 = DAT_1080_259c;
  LOCK();
  DAT_1080_259c = 0x1000;
  UNLOCK();
  iVar2 = FUN_1000_130d();
  DAT_1080_259c = uVar1;
  if (in_DX != 0 || iVar2 != 0) {
    return;
  }
  FUN_1000_05b4();
  return;
}



/* ============================================================
   1000:0698  FUN_1000_0698
   ============================================================ */

undefined2 __cdecl16far FUN_1000_0698(void)

{
  bool in_CF;
  
  if (!in_CF) {
    return 0;
  }
  FUN_1000_06cf();
  return 0xffff;
}



/* ============================================================
   1000:06a5  FUN_1000_06a5
   ============================================================ */

undefined1 __cdecl16far FUN_1000_06a5(void)

{
  undefined1 in_AL;
  bool in_CF;
  
  if (in_CF) {
    FUN_1000_06cf();
    return in_AL;
  }
  return 0;
}



/* ============================================================
   1000:06b7  FUN_1000_06b7
   ============================================================ */

void __cdecl16far FUN_1000_06b7(void)

{
  bool in_CF;
  
  if (in_CF) {
    FUN_1000_06cf();
  }
  return;
}



/* ============================================================
   1000:06cf  FUN_1000_06cf
   ============================================================ */

void __cdecl16near FUN_1000_06cf(void)

{
  char cVar1;
  uint in_AX;
  
  DAT_1080_24bc = (byte)in_AX;
  cVar1 = (char)(in_AX >> 8);
  if (cVar1 != '\0') goto LAB_1000_06ec;
  if (DAT_1080_24bc < 0x22) {
    if (DAT_1080_24bc < 0x20) {
      if (0x13 < DAT_1080_24bc) goto LAB_1000_06e6;
    }
    else {
      in_AX = 5;
    }
  }
  else {
LAB_1000_06e6:
    in_AX = 0x13;
  }
  cVar1 = *(char *)(ulong)((in_AX & 0xff) + 0x250a);
LAB_1000_06ec:
  DAT_1080_24ac = (int)cVar1;
  return;
}



/* ============================================================
   1000:06f6  FUN_1000_06f6
   ============================================================ */

undefined2 __cdecl16far FUN_1000_06f6(void)

{
  undefined2 unaff_SS;
  
  if (___EXPORTEDSTUB != (code)0xb8) {
    return unaff_SS;
  }
  return uRam10000707;
}



/* ============================================================
   1000:0706  ___EXPORTEDSTUB
   ============================================================ */

undefined2 __cdecl16far ___EXPORTEDSTUB(void)

{
  return 0;
}



/* ============================================================
   1000:071a  FUN_1000_071a
   ============================================================ */

undefined2 __cdecl16far FUN_1000_071a(int param_1)

{
  bool bVar1;
  int iVar2;
  int unaff_BP;
  undefined2 uVar3;
  undefined2 unaff_SS;
  undefined1 *local_14;
  char local_10;
  undefined1 uStack_f;
  undefined1 local_e [8];
  int local_6;
  undefined2 local_4;
  int iStack_2;
  
  iStack_2 = unaff_BP + 1;
  local_4 = 0x1080;
  uVar3 = 0xffff;
  if ((*(byte *)(param_1 + 10) & 0x40) != 0) {
    *(undefined1 *)(param_1 + 10) = 0;
    return 0xffff;
  }
  if ((*(byte *)(param_1 + 10) & 0x83) == 0) goto LAB_1000_080c;
  uVar3 = FUN_1000_0a0c(param_1,0x1080);
  local_6 = *(int *)(param_1 + 0xf4);
  FUN_1000_092c(param_1,0x1080);
  if (DAT_1080_24be < (int)(uint)*(byte *)(param_1 + 0xb)) {
    iVar2 = FUN_1000_20c9();
    if (iVar2 < 0) goto LAB_1000_0784;
LAB_1000_079c:
    bVar1 = false;
  }
  else {
    iVar2 = FUN_1000_1012();
    if (-1 < iVar2) goto LAB_1000_079c;
LAB_1000_0784:
    bVar1 = true;
  }
  if (!bVar1) {
    if (local_6 == 0) goto LAB_1000_080c;
    FUN_1000_16aa(&local_10);
    local_14 = local_e;
    if (local_10 == '\\') {
      local_14 = &uStack_f;
    }
    else {
      FUN_1000_1656(&local_10);
    }
    FUN_1000_1822(0x1000,local_6,local_14,unaff_SS,10);
    iVar2 = FUN_1000_1fa8(0x1000,&local_10);
    if (iVar2 == 0) goto LAB_1000_080c;
  }
  uVar3 = 0xffff;
LAB_1000_080c:
  *(undefined1 *)(param_1 + 10) = 0;
  return uVar3;
}



/* ============================================================
   1000:081c  FUN_1000_081c
   ============================================================ */

void __cdecl16far FUN_1000_081c(void)

{
  FUN_1000_0a8c();
  if (DAT_1080_24fd != '\0') {
    FUN_1000_18e4();
  }
  return;
}



/* ============================================================
   1000:0832  FUN_1000_0832
   ============================================================ */

uint __cdecl16far FUN_1000_0832(byte param_1,int *param_2)

{
  byte bVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  
  bVar2 = *(byte *)(param_2 + 5);
  if (((bVar2 & 0x82) != 0) && ((bVar2 & 0x40) == 0)) {
    param_2[2] = 0;
    if ((bVar2 & 1) != 0) {
      if ((bVar2 & 0x10) == 0) goto LAB_1000_08b3;
      *param_2 = param_2[3];
      bVar2 = bVar2 & 0xfe;
    }
    *(byte *)(param_2 + 5) = bVar2 & 0xef | 2;
    bVar1 = *(byte *)((int)param_2 + 0xb);
    if (((bVar2 & 8) == 0) &&
       (((bVar2 & 4) != 0 ||
        (((*(byte *)(param_2 + 0x78) & 1) == 0 &&
         (((DAT_1080_2612 != 0 &&
           (((param_2 == (int *)0x2632 || (param_2 == (int *)0x263e)) &&
            ((*(byte *)(bVar1 + 0x24c4) & 0x40) != 0)))) ||
          (FUN_1000_0964(param_2,0x1080), (*(byte *)(param_2 + 5) & 8) == 0)))))))) {
      iVar3 = FUN_1000_111e();
      iVar4 = 1;
    }
    else {
      iVar4 = *param_2 - param_2[3];
      *param_2 = param_2[3] + 1;
      param_2[2] = param_2[0x79] + -1;
      if (iVar4 == 0) {
        iVar3 = 0;
        if ((*(byte *)(bVar1 + 0x24c4) & 0x20) != 0) {
          FUN_1000_104a();
          iVar3 = 0;
          iVar4 = 0;
        }
      }
      else {
        iVar3 = FUN_1000_111e();
      }
      **(byte **)(param_2 + 3) = param_1;
    }
    if (iVar3 == iVar4) {
      return (uint)param_1;
    }
  }
LAB_1000_08b3:
  *(byte *)(param_2 + 5) = *(byte *)(param_2 + 5) | 0x20;
  return 0xffff;
}



/* ============================================================
   1000:092c  FUN_1000_092c
   ============================================================ */

void __cdecl16near FUN_1000_092c(undefined2 *param_1)

{
  if (((*(byte *)(param_1 + 5) & 0x83) != 0) && ((*(byte *)(param_1 + 5) & 8) != 0)) {
    FUN_1000_12ec(param_1[3],param_1[4]);
    *(byte *)(param_1 + 5) = *(byte *)(param_1 + 5) & 0xf7;
    param_1[3] = 0;
    param_1[4] = 0;
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
  }
  return;
}



/* ============================================================
   1000:0964  FUN_1000_0964
   ============================================================ */

void __cdecl16near FUN_1000_0964(int *param_1)

{
  int iVar1;
  int in_DX;
  
  iVar1 = FUN_1000_130d(0x200);
  if (in_DX == 0) {
    *(byte *)(param_1 + 5) = *(byte *)(param_1 + 5) | 4;
    param_1[0x79] = 1;
    in_DX = 0x1080;
    iVar1 = (int)param_1 + 0xf1;
  }
  else {
    *(byte *)(param_1 + 5) = *(byte *)(param_1 + 5) | 8;
    param_1[0x79] = 0x200;
  }
  param_1[1] = in_DX;
  *param_1 = iVar1;
  param_1[4] = in_DX;
  param_1[3] = iVar1;
  param_1[2] = 0;
  return;
}



/* ============================================================
   1000:09b0  FUN_1000_09b0
   ============================================================ */

int __cdecl16far FUN_1000_09b0(undefined4 param_1)

{
  int iVar1;
  
  if (param_1._2_2_ == 0 && (int)param_1 == 0) {
    iVar1 = FUN_1000_0aa0(0);
  }
  else {
    iVar1 = FUN_1000_0a0c((int)param_1,param_1._2_2_);
    if (iVar1 == 0) {
      iVar1 = 0;
      if ((*(byte *)((int)param_1 + 0xf0) & 0x40) != 0) {
        iVar1 = FUN_1000_1992(*(undefined1 *)((int)param_1 + 0xb));
        iVar1 = -(uint)(iVar1 != 0);
      }
    }
    else {
      iVar1 = -1;
    }
  }
  return iVar1;
}



/* ============================================================
   1000:0a0c  FUN_1000_0a0c
   ============================================================ */

undefined2 __cdecl16near FUN_1000_0a0c(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  
  uVar4 = 0;
  if (((*(byte *)(param_1 + 5) & 3) == 2) &&
     (((*(byte *)(param_1 + 5) & 8) != 0 || ((*(byte *)(param_1 + 0x78) & 1) != 0)))) {
    iVar1 = *param_1;
    iVar2 = param_1[3];
    if (0 < iVar1 - iVar2) {
      iVar3 = FUN_1000_111e();
      if (iVar3 == iVar1 - iVar2) {
        if ((*(byte *)(param_1 + 5) & 0x80) != 0) {
          *(byte *)(param_1 + 5) = *(byte *)(param_1 + 5) & 0xfd;
        }
      }
      else {
        *(byte *)(param_1 + 5) = *(byte *)(param_1 + 5) | 0x20;
        uVar4 = 0xffff;
      }
    }
  }
  iVar1 = param_1[4];
  *param_1 = param_1[3];
  param_1[1] = iVar1;
  param_1[2] = 0;
  return uVar4;
}



/* ============================================================
   1000:0a8c  FUN_1000_0a8c
   ============================================================ */

void __cdecl16far FUN_1000_0a8c(void)

{
  int unaff_BP;
  
  FUN_1000_0aa0(1,0x1080,unaff_BP + 1);
  return;
}



/* ============================================================
   1000:0aa0  FUN_1000_0aa0
   ============================================================ */

int FUN_1000_0aa0(int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int local_4;
  
  iVar3 = 0;
  local_4 = 0;
  for (uVar2 = 0x2626; uVar2 <= DAT_1080_2524; uVar2 = uVar2 + 0xc) {
    if ((param_1 == 1) && ((*(byte *)(uVar2 + 10) & 0x83) != 0)) {
      iVar1 = FUN_1000_09b0(uVar2,0x1080);
      if (iVar1 != -1) {
        iVar3 = iVar3 + 1;
      }
    }
    else if ((param_1 == 0) &&
            (((*(byte *)(uVar2 + 10) & 2) != 0 && (iVar1 = FUN_1000_09b0(uVar2,0x1080), iVar1 == -1)
             ))) {
      local_4 = -1;
    }
  }
  if (param_1 == 1) {
    local_4 = iVar3;
  }
  return local_4;
}



/* ============================================================
   1000:0b1c  FUN_1000_0b1c
   ============================================================ */

undefined2 __cdecl16far FUN_1000_0b1c(undefined2 param_1,undefined2 param_2,char *param_3)

{
  char cVar1;
  byte bVar2;
  undefined2 uVar3;
  int unaff_BP;
  
  FUN_1000_030c(0x1080,unaff_BP + 1);
  cVar1 = *param_3;
  if (cVar1 == '\0') {
    return 0;
  }
  if ((byte)(cVar1 - 0x20U) < 0x59) {
    bVar2 = *(byte *)(ulong)((byte)(cVar1 - 0x20U) + 0x2526) & 0xf;
  }
  else {
    bVar2 = 0;
  }
                    /* WARNING: Could not emulate address calculation at 0x10000b69 */
                    /* WARNING: Treating indirect jump as call */
  uVar3 = (*(code *)*(undefined2 *)
                     ((char)(*(byte *)(ulong)((byte)(bVar2 * '\b') + 0x2526) >> 4) * 2 + 0xb0c))
                    (cVar1);
  return uVar3;
}



/* ============================================================
   1000:1012  FUN_1000_1012
   ============================================================ */

void FUN_1000_1012(undefined2 param_1,uint param_2)

{
  code *pcVar1;
  int unaff_BP;
  undefined1 uVar2;
  
  if (param_2 < DAT_1080_24be) {
    uVar2 = 0;
    if ((DAT_1000_0010 & 1) == 0) {
      pcVar1 = (code *)swi(0x21);
      (*pcVar1)(unaff_BP + 1);
    }
    else {
      DOS3CALL(0x1000,0x1080);
    }
    if (!(bool)uVar2) {
      *(undefined1 *)(param_2 + 0x24c4) = 0;
    }
  }
  FUN_1000_0698();
  return;
}



/* ============================================================
   1000:104a  FUN_1000_104a
   ============================================================ */

void FUN_1000_104a(undefined2 param_1,uint param_2,uint param_3,uint param_4,uint param_5)

{
  code *pcVar1;
  undefined1 uVar2;
  undefined4 uVar3;
  
  if (((DAT_1080_24be <= param_2) && (DAT_1080_2612 != 0)) && (param_2 < 3)) goto LAB_1000_111b;
  if ((DAT_1080_2588 != 0) && ((param_4 & 0x8000) != 0)) {
    if (param_5 == 0) goto LAB_1000_111b;
    uVar2 = 0;
    if ((DAT_1000_0010 & 1) == 0) {
      pcVar1 = (code *)swi(0x21);
      uVar3 = (*pcVar1)();
    }
    else {
      uVar3 = DOS3CALL(0x1000);
    }
    if ((bool)uVar2) goto LAB_1000_111b;
    if ((param_5 & 2) == 0) {
      if ((int)((int)((ulong)uVar3 >> 0x10) + param_4 + (uint)CARRY2((uint)uVar3,param_3)) < 0)
      goto LAB_1000_111b;
    }
    else {
      if ((DAT_1000_0010 & 1) == 0) {
        pcVar1 = (code *)swi(0x21);
        uVar3 = (*pcVar1)(uVar3);
      }
      else {
        uVar3 = DOS3CALL(0x1000);
      }
      if ((int)((int)((ulong)uVar3 >> 0x10) + param_4 + (uint)CARRY2((uint)uVar3,param_3)) < 0) {
        if ((DAT_1000_0010 & 1) == 0) {
          pcVar1 = (code *)swi(0x21);
          (*pcVar1)();
        }
        else {
          DOS3CALL(0x1000);
        }
        goto LAB_1000_111b;
      }
    }
  }
  uVar2 = 0;
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
  }
  else {
    DOS3CALL();
  }
  if (!(bool)uVar2) {
    *(byte *)(param_2 + 0x24c4) = *(byte *)(param_2 + 0x24c4) & 0xfd;
  }
LAB_1000_111b:
  FUN_1000_06b7();
  return;
}



/* ============================================================
   1000:111e  FUN_1000_111e
   ============================================================ */

/* WARNING: Unable to track spacebase fully for stack */

uint FUN_1000_111e(undefined2 param_1,uint param_2,char *param_3,int param_4)

{
  char *pcVar1;
  code *pcVar2;
  char cVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  undefined2 uVar7;
  int iVar8;
  char *pcVar9;
  char *pcVar10;
  char *pcVar11;
  undefined2 unaff_SS;
  undefined1 uVar12;
  bool bVar13;
  uint in_stack_0000ffde;
  
  uVar5 = DAT_1080_24be;
  uVar4 = DAT_1080_24be;
  if ((DAT_1080_2612 != 0) && (uVar4 = DAT_1080_24c2, param_2 < 3)) {
    param_2 = DAT_1080_24be;
  }
  if (uVar4 <= param_2) {
LAB_1000_1152:
    uVar5 = FUN_1000_06b7();
    return uVar5;
  }
  uVar4 = param_2;
  if ((*(byte *)(param_2 + 0x24c4) & 0x20) != 0) {
    uVar12 = 0;
    if ((DAT_1000_0010 & 1) == 0) {
      pcVar2 = (code *)swi(0x21);
      (*pcVar2)();
    }
    else {
      DOS3CALL();
    }
    if ((bool)uVar12) goto LAB_1000_1152;
  }
  if ((*(byte *)(uVar4 + 0x24c4) & 0x80) != 0) {
    pcVar9 = (char *)param_3;
    bVar13 = true;
    iVar8 = param_4;
    pcVar11 = pcVar9;
    if (param_4 != 0) {
      do {
        if (iVar8 == 0) break;
        iVar8 = iVar8 + -1;
        pcVar1 = pcVar11;
        pcVar11 = pcVar11 + 1;
        bVar13 = *pcVar1 == '\n';
      } while (!bVar13);
      if (!bVar13) goto LAB_1000_11fb;
      pcVar10 = pcVar9;
      uVar6 = FUN_1000_12d8();
      if (uVar6 < 0xa9) {
        uVar7 = FUN_1000_030c();
        if ((int)pcVar11 - (int)pcVar10 != 0) {
          uVar12 = param_2 < uVar5;
          if ((bool)uVar12) {
            uVar12 = 0;
            if ((DAT_1000_0010 & 1) == 0) {
              pcVar2 = (code *)swi(0x21);
              uVar5 = (*pcVar2)(0x1080,iVar8,uVar4,uVar7);
            }
            else {
              in_stack_0000ffde = 0x1000;
              uVar5 = DOS3CALL(0x1000,(int)pcVar11 - (int)pcVar10);
            }
          }
          else {
            in_stack_0000ffde = 0x121d;
            uVar5 = FUN_1000_20c9();
          }
          if (((bool)uVar12) || (uVar5 < in_stack_0000ffde)) {
            uVar5 = FUN_1000_06b7();
            return uVar5;
          }
        }
        return uVar4;
      }
      pcVar10 = &stack0xffea;
      pcVar11 = &stack0xffec;
      do {
        pcVar1 = pcVar9;
        pcVar9 = pcVar9 + 1;
        cVar3 = *pcVar1;
        if (cVar3 == '\n') {
          cVar3 = '\r';
          if (pcVar11 == pcVar10) {
            cVar3 = FUN_1000_1205();
          }
          pcVar1 = pcVar11;
          pcVar11 = pcVar11 + 1;
          *pcVar1 = cVar3;
          cVar3 = '\n';
        }
        if (pcVar11 == pcVar10) {
          cVar3 = FUN_1000_1205();
        }
        pcVar1 = pcVar11;
        pcVar11 = pcVar11 + 1;
        *pcVar1 = cVar3;
        param_4 = param_4 + -1;
      } while (param_4 != 0);
      FUN_1000_1205();
    }
    uVar5 = FUN_1000_1274();
    return uVar5;
  }
LAB_1000_11fb:
  uVar5 = FUN_1000_1285();
  return uVar5;
}



/* ============================================================
   1000:1205  FUN_1000_1205
   ============================================================ */

/* WARNING: Unable to track spacebase fully for stack */

undefined2 __cdecl16near FUN_1000_1205(void)

{
  code *pcVar1;
  uint uVar2;
  undefined2 uVar3;
  int in_DX;
  undefined2 in_BX;
  int unaff_BP;
  int unaff_DI;
  undefined2 unaff_SS;
  undefined1 uVar4;
  uint in_stack_0000fff4;
  
  if (unaff_DI - in_DX != 0) {
    uVar4 = *(uint *)(unaff_BP + 6) < *(uint *)(unaff_BP + -0xc);
    if ((bool)uVar4) {
      uVar4 = 0;
      if ((DAT_1000_0010 & 1) == 0) {
        pcVar1 = (code *)swi(0x21);
        uVar2 = (*pcVar1)(0x1080);
      }
      else {
        in_stack_0000fff4 = 0x1000;
        uVar2 = DOS3CALL(0x1000,unaff_DI - in_DX);
      }
    }
    else {
      in_stack_0000fff4 = 0x121d;
      uVar2 = FUN_1000_20c9();
    }
    if (((bool)uVar4) ||
       (*(int *)(unaff_BP + -4) = *(int *)(unaff_BP + -4) + uVar2, uVar2 < in_stack_0000fff4)) {
      uVar3 = FUN_1000_06b7();
      return uVar3;
    }
  }
  return in_BX;
}



/* ============================================================
   1000:1274  FUN_1000_1274
   ============================================================ */

/* WARNING: Stack frame is not setup normally: Input value of stackpointer is not used */

void FUN_1000_1274(void)

{
  FUN_1000_06b7();
  return;
}



/* ============================================================
   1000:1285  FUN_1000_1285
   ============================================================ */

void FUN_1000_1285(void)

{
  code *pcVar1;
  uint in_BX;
  int unaff_BP;
  undefined2 unaff_SS;
  
  if (*(int *)(unaff_BP + 0xc) != 0) {
    if (in_BX < *(uint *)(unaff_BP + -0xc)) {
      if ((DAT_1000_0010 & 1) == 0) {
        pcVar1 = (code *)swi(0x21);
        (*pcVar1)();
      }
      else {
        DOS3CALL(0x1000,0x1080);
      }
    }
    else {
      FUN_1000_20c9();
    }
    FUN_1000_06b7();
    return;
  }
  FUN_1000_06b7();
  return;
}



/* ============================================================
   1000:12d8  FUN_1000_12d8
   ============================================================ */

int __cdecl16far FUN_1000_12d8(void)

{
  int iVar1;
  
  if (DAT_1080_247c < &stack0x0004) {
    iVar1 = -((int)DAT_1080_247c - (int)&stack0x0004);
  }
  else {
    iVar1 = 0;
  }
  return iVar1;
}



/* ============================================================
   1000:12ec  FUN_1000_12ec
   ============================================================ */

undefined2 __cdecl16far FUN_1000_12ec(undefined4 param_1)

{
  byte *pbVar1;
  int iVar2;
  
  iVar2 = (int)((ulong)param_1 >> 0x10);
  if (iVar2 != 0) {
    pbVar1 = (byte *)((int)param_1 + -2);
    *pbVar1 = *pbVar1 | 1;
  }
  return 0x1080;
}



/* ============================================================
   1000:130d  FUN_1000_130d
   ============================================================ */

undefined2 __cdecl16far FUN_1000_130d(uint param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int in_BX;
  uint uVar3;
  code *pcVar4;
  undefined2 unaff_CS;
  undefined2 uVar5;
  uint uVar6;
  bool bVar7;
  bool bVar8;
  undefined4 uVar9;
  uint uVar10;
  
  uVar5 = 0x1080;
  do {
    if (param_1 < 0xffe7) {
      bVar7 = false;
      if (*(int *)((int)(undefined4 *)&DAT_1080_258e + 2) != 0) {
        pcVar4 = (code *)0x141e;
        while( true ) {
          uVar3 = *(uint *)&DAT_1080_2598;
          uVar6 = (uint)((ulong)*(undefined4 *)&DAT_1080_2592 >> 0x10);
          in_BX = (int)*(undefined4 *)&DAT_1080_2592;
          uVar10 = uVar6;
          do {
            do {
              unaff_CS = 0x1000;
              uVar9 = (*pcVar4)();
              if (!bVar7) {
                if (pcVar4 == (code *)0x141e) goto LAB_1000_137e;
                goto LAB_1000_137b;
              }
              puVar1 = (undefined4 *)(in_BX + 0xe);
              in_BX = (int)*puVar1;
              bVar7 = uVar6 < uVar3;
              bVar8 = uVar6 != uVar3;
              uVar6 = (uint)((ulong)*puVar1 >> 0x10);
            } while (bVar8);
            uVar3 = *(uint *)((int)*(undefined4 *)&DAT_1080_2592 + 0x14);
            uVar6 = (uint)((ulong)*(undefined4 *)&DAT_1080_258e >> 0x10);
            in_BX = (int)*(undefined4 *)&DAT_1080_258e;
            bVar7 = uVar6 < uVar10;
            bVar8 = uVar6 != uVar10;
            uVar10 = uVar6;
          } while (bVar8);
          bVar7 = pcVar4 < (code *)0x149a;
          if (pcVar4 == (code *)0x149a) break;
          pcVar4 = (code *)0x149a;
        }
      }
      FUN_1000_15b2();
      if (!bVar7) {
LAB_1000_137b:
        uVar9 = FUN_1000_141e();
LAB_1000_137e:
        *(undefined2 *)((int)(undefined4 *)&DAT_1080_2592 + 2) = (int)((ulong)uVar9 >> 0x10);
        *(int *)&DAT_1080_2592 = in_BX;
        return (int)uVar9;
      }
    }
    if (*(int *)&DAT_1080_258c == 0 && *(int *)&DAT_1080_258a == 0) {
      return 0;
    }
    iVar2 = (*(code *)*(undefined2 *)&DAT_1080_258a)(unaff_CS,param_1);
    if (iVar2 == 0) {
      return 0;
    }
  } while( true );
}



/* ============================================================
   1000:13b4  FUN_1000_13b4
   ============================================================ */

void __cdecl16near FUN_1000_13b4(void)

{
  int *piVar1;
  int in_AX;
  undefined2 *in_BX;
  undefined2 *puVar2;
  
  in_BX[2] = in_AX + (int)in_BX;
  puVar2 = (undefined2 *)(in_AX + (int)in_BX + -2);
  piVar1 = in_BX + 0xb;
  *puVar2 = 0xfffe;
  in_BX[6] = puVar2;
  *piVar1 = in_AX + -0x19;
  *in_BX = 0x1080;
  in_BX[4] = piVar1;
  in_BX[5] = piVar1;
  in_BX[7] = 0;
  in_BX[8] = 0;
  in_BX[9] = 0;
  in_BX[10] = 0;
  return;
}



/* ============================================================
   1000:13e8  FUN_1000_13e8
   ============================================================ */

void __cdecl16near FUN_1000_13e8(void)

{
  int in_BX;
  int iVar1;
  int *unaff_DI;
  undefined2 unaff_ES;
  undefined2 uVar2;
  
  if (unaff_DI[1] == 0) {
    unaff_DI[1] = 0x1080;
    *unaff_DI = in_BX;
  }
  else {
    uVar2 = (undefined2)((ulong)*(undefined4 *)(unaff_DI + 4) >> 0x10);
    iVar1 = (int)*(undefined4 *)(unaff_DI + 4);
    *(undefined2 *)(iVar1 + 0x10) = 0x1080;
    *(int *)(iVar1 + 0xe) = in_BX;
    *(undefined2 *)(in_BX + 0x14) = uVar2;
    *(int *)(in_BX + 0x12) = iVar1;
  }
  unaff_DI[5] = 0x1080;
  unaff_DI[4] = in_BX;
  unaff_DI[3] = 0x1080;
  unaff_DI[2] = in_BX;
  return;
}



/* ============================================================
   1000:141e  FUN_1000_141e
   ============================================================ */

uint * __cdecl16near FUN_1000_141e(void)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  int in_CX;
  uint uVar4;
  int in_BX;
  uint *puVar5;
  uint *puVar6;
  uint *puVar7;
  
  uVar4 = in_CX + 1U & 0xfffe;
  puVar7 = (uint *)*(undefined2 *)(in_BX + 10);
  puVar5 = (uint *)*(undefined2 *)(in_BX + 0xc);
  do {
    while( true ) {
      puVar1 = puVar7 + 1;
      uVar3 = *puVar7;
      puVar6 = puVar1;
      if ((uVar3 & 1) != 0) {
        while( true ) {
          uVar2 = uVar3 - 1;
          if (uVar4 <= uVar2) {
            *puVar7 = uVar4;
            puVar7 = puVar1;
            if (uVar2 != uVar4) {
              *(int *)((int)puVar1 + uVar4) = (uVar2 - uVar4) + -1;
              puVar7 = (uint *)((int)((int)puVar1 + uVar4) - uVar4);
            }
            *(int *)(in_BX + 10) = (int)puVar7 + uVar4;
            return puVar1;
          }
          if (CARRY2((uint)puVar1,uVar2)) goto LAB_1000_1477;
          puVar6 = (uint *)((int)puVar1 + uVar2) + 1;
          uVar3 = *(uint *)((int)puVar1 + uVar2);
          if ((uVar3 & 1) == 0) break;
          uVar3 = uVar3 + uVar2 + 2;
          *puVar7 = uVar3;
        }
      }
      if (puVar6 + -1 < puVar5) break;
      if (((uint)puVar5 & 1) != 0) goto LAB_1000_1477;
      puVar7 = (uint *)*(undefined2 *)(in_BX + 8);
      if ((uint *)*(undefined2 *)(in_BX + 10) == puVar7) goto LAB_1000_1477;
      puVar5 = (uint *)((int)*(undefined2 *)(in_BX + 10) + -1);
    }
    puVar7 = (uint *)((int)puVar6 + uVar3);
  } while (!CARRY2((uint)puVar6,uVar3));
LAB_1000_1477:
  puVar7 = (uint *)*(undefined2 *)(in_BX + 8);
  *(undefined2 *)(in_BX + 10) = puVar7;
  return puVar7;
}



/* ============================================================
   1000:149a  FUN_1000_149a
   ============================================================ */

void __cdecl16near FUN_1000_149a(void)

{
  int *piVar1;
  undefined2 uVar2;
  int iVar3;
  uint uVar4;
  int in_CX;
  uint uVar5;
  int iVar6;
  int in_BX;
  uint *unaff_SI;
  undefined2 *puVar7;
  bool bVar8;
  
  if ((*(byte *)(in_BX + 2) & 1) != 0) {
    FUN_1000_1591();
    if ((*unaff_SI & 1) != 0) {
      in_CX = (in_CX - *unaff_SI) + -1;
    }
    uVar4 = *(uint *)(in_BX + 4);
    if (uVar4 != 0) {
      if (!CARRY2(in_CX + 2U,uVar4)) {
        uVar2 = FUN_1000_06f6();
        uVar4 = *(uint *)&DAT_1080_259c;
        if (uVar4 == 0x1000) goto LAB_1000_14ec;
        uVar5 = 0x8000;
        while (uVar4 <= uVar5) {
          uVar5 = uVar5 >> 1;
          if (uVar5 == 0) goto LAB_1000_1505;
        }
        if (uVar5 < 8) goto LAB_1000_1505;
        uVar4 = uVar5 << 1;
        goto LAB_1000_14ec;
      }
      uVar5 = 0xfff0;
      if (in_CX + 2U + uVar4 == 0) {
        while( true ) {
          bVar8 = false;
          iVar3 = FUN_1000_152b();
          if (!bVar8) break;
          if (uVar5 == 0xfff0) {
            return;
          }
LAB_1000_1505:
          uVar4 = 0x10;
LAB_1000_14ec:
          uVar5 = ~(uVar4 - 1);
        }
        iVar6 = iVar3 - *(int *)(in_BX + 4);
        *(int *)(in_BX + 4) = iVar3;
        *(undefined2 *)(in_BX + 10) = unaff_SI;
        piVar1 = (int *)*(int *)(in_BX + 0xc);
        *piVar1 = iVar6 + -1;
        puVar7 = (undefined2 *)((int)piVar1 + iVar6);
        *puVar7 = 0xfffe;
        *(undefined2 *)(in_BX + 0xc) = puVar7;
      }
    }
  }
  return;
}



/* ============================================================
   1000:152b  FUN_1000_152b
   ============================================================ */

void __cdecl16near FUN_1000_152b(void)

{
  int in_AX;
  undefined2 uVar1;
  int iVar2;
  undefined2 uVar3;
  int in_BX;
  long lVar4;
  int iVar5;
  int iVar6;
  
  if ((*(byte *)(in_BX + 2) & 4) != 0) {
LAB_1000_1586:
    FUN_1000_05b4();
    return;
  }
  iVar5 = *(int *)(in_BX + 6);
  uVar3 = 0;
  if (in_AX == 0) {
    if ((DAT_1000_0010 & 0x10) != 0) {
      return;
    }
    uVar3 = 1;
  }
  uVar1 = 2;
  if ((DAT_1000_0010 & 1) == 0) {
    uVar1 = 0x20;
  }
  iVar6 = iVar5;
  iVar2 = GLOBALREALLOC(0x1000,uVar1,in_AX,uVar3);
  if (iVar2 != 0) {
    if ((iVar2 != iVar5) || (lVar4 = GLOBALSIZE(0x11d0,iVar5,iVar6), lVar4 == 0))
    goto LAB_1000_1586;
    if ((*(byte *)(iVar5 + 2) & 4) != 0) {
      *(int *)(iVar5 + -2) = in_BX + -1;
    }
  }
  return;
}



/* ============================================================
   1000:1591  FUN_1000_1591
   ============================================================ */

void __cdecl16near FUN_1000_1591(void)

{
  int in_BX;
  uint *puVar1;
  
  puVar1 = (uint *)*(undefined2 *)(in_BX + 10);
  if (puVar1 == (uint *)*(undefined2 *)(in_BX + 0xc)) {
    puVar1 = (uint *)*(undefined2 *)(in_BX + 8);
  }
  while( true ) {
    if (*puVar1 == 0xfffe) break;
    puVar1 = (uint *)((int)puVar1 + (*puVar1 & 0xfffe) + 2);
  }
  return;
}



/* ============================================================
   1000:15b2  FUN_1000_15b2
   ============================================================ */

void __cdecl16near FUN_1000_15b2(void)

{
  int iVar1;
  int in_CX;
  int iVar2;
  uint uVar3;
  int unaff_DI;
  undefined4 uVar4;
  long lVar5;
  uint uVar6;
  uint uVar7;
  
  uVar3 = in_CX + 0x1019U & 0xf000;
  iVar2 = 0;
  if (uVar3 == 0) {
    if ((DAT_1000_0010 & 0x10) != 0) {
      return;
    }
    iVar2 = 1;
  }
  uVar6 = 0;
  uVar7 = DAT_1000_0010;
  iVar1 = GLOBALALLOC(0x1000,uVar3,iVar2);
  if (iVar1 != 0) {
    if ((uVar6 & 1) != 0) {
      iVar2 = iVar1;
      uVar4 = GLOBALLOCK(0x11d0,iVar1,iVar1,uVar7);
      iVar1 = (int)((ulong)uVar4 >> 0x10);
      if (((int)uVar4 != 0) || (iVar1 == 0)) goto LAB_1000_1629;
    }
    lVar5 = GLOBALSIZE(0x11d0,iVar1);
    if (lVar5 == 0) {
LAB_1000_1629:
      FUN_1000_05b4();
      return;
    }
    *(int *)0x6 = iVar2;
    *(undefined2 *)0x2 = *(undefined2 *)(unaff_DI + 0xc);
    FUN_1000_13b4();
    FUN_1000_13e8();
  }
  return;
}



/* ============================================================
   1000:1634  FUN_1000_1634
   ============================================================ */

void __cdecl16far FUN_1000_1634(undefined2 param_1,undefined2 param_2)

{
  FUN_1000_12ec(param_1,param_2);
  return;
}



/* ============================================================
   1000:1646  FUN_1000_1646
   ============================================================ */

void __cdecl16far FUN_1000_1646(undefined2 param_1)

{
  FUN_1000_130d(param_1);
  return;
}



/* ============================================================
   1000:1656  FUN_1000_1656
   ============================================================ */

char * __cdecl16far FUN_1000_1656(char *param_1,char *param_2)

{
  char *pcVar1;
  char *pcVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  char *pcVar7;
  char *pcVar8;
  undefined2 uVar9;
  undefined2 uVar10;
  bool bVar11;
  
  uVar9 = (undefined2)((ulong)param_1 >> 0x10);
  bVar11 = true;
  iVar3 = -1;
  pcVar6 = (char *)param_1;
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    pcVar1 = pcVar6;
    pcVar6 = pcVar6 + 1;
    bVar11 = *pcVar1 == '\0';
  } while (!bVar11);
  pcVar8 = pcVar6 + -1;
  uVar10 = (undefined2)((ulong)param_2 >> 0x10);
  pcVar7 = (char *)param_2;
  uVar4 = 0xffff;
  do {
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar1 = pcVar7;
    pcVar7 = pcVar7 + 1;
    bVar11 = *pcVar1 == '\0';
  } while (!bVar11);
  uVar4 = ~uVar4;
  if (!bVar11) {
    pcVar7 = pcVar7 + -uVar4;
    uVar4 = uVar4 + 1;
  }
  pcVar7 = pcVar7 + -uVar4;
  if (uVar4 == 0) {
    pcVar1 = pcVar7;
    pcVar7 = pcVar7 + 2;
    *(undefined2 *)pcVar8 = *(undefined2 *)pcVar1;
    uVar4 = 0xfffe;
    pcVar8 = pcVar6 + 1;
  }
  else if (((uint)pcVar7 & 1) != 0) {
    pcVar1 = pcVar7;
    pcVar7 = pcVar7 + 1;
    *pcVar8 = *pcVar1;
    uVar4 = uVar4 - 1;
    pcVar8 = pcVar6;
  }
  for (uVar5 = uVar4 >> 1; uVar5 != 0; uVar5 = uVar5 - 1) {
    pcVar2 = pcVar8;
    pcVar8 = pcVar8 + 2;
    pcVar1 = pcVar7;
    pcVar7 = pcVar7 + 2;
    *(undefined2 *)pcVar2 = *(undefined2 *)pcVar1;
  }
  for (uVar4 = (uint)((uVar4 & 1) != 0); uVar4 != 0; uVar4 = uVar4 - 1) {
    pcVar2 = pcVar8;
    pcVar8 = pcVar8 + 1;
    pcVar1 = pcVar7;
    pcVar7 = pcVar7 + 1;
    *pcVar2 = *pcVar1;
  }
  return (char *)param_1;
}



/* ============================================================
   1000:16aa  FUN_1000_16aa
   ============================================================ */

void __cdecl16far FUN_1000_16aa(char *param_1,char *param_2)

{
  char *pcVar1;
  char *pcVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  char *pcVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  bool bVar9;
  
  uVar8 = (undefined2)((ulong)param_2 >> 0x10);
  pcVar5 = (char *)param_2;
  bVar9 = true;
  uVar3 = 0xffff;
  pcVar6 = pcVar5;
  do {
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    pcVar1 = pcVar6;
    pcVar6 = pcVar6 + 1;
    bVar9 = *pcVar1 == '\0';
  } while (!bVar9);
  uVar3 = ~uVar3;
  uVar7 = (undefined2)((ulong)param_1 >> 0x10);
  pcVar6 = (char *)param_1;
  if (bVar9) {
    if (((ulong)param_1 & 1) != 0) {
      pcVar6 = pcVar6 + 1;
      pcVar5 = pcVar5 + 1;
      *param_1 = *param_2;
      uVar3 = uVar3 - 1;
    }
  }
  else {
    pcVar6 = pcVar6 + 2;
    pcVar5 = pcVar5 + 2;
    *(undefined2 *)param_1 = *(undefined2 *)param_2;
    uVar3 = uVar3 - 1;
  }
  for (uVar4 = uVar3 >> 1; uVar4 != 0; uVar4 = uVar4 - 1) {
    pcVar2 = pcVar6;
    pcVar6 = pcVar6 + 2;
    pcVar1 = pcVar5;
    pcVar5 = pcVar5 + 2;
    *(undefined2 *)pcVar2 = *(undefined2 *)pcVar1;
  }
  for (uVar3 = (uint)((uVar3 & 1) != 0); uVar3 != 0; uVar3 = uVar3 - 1) {
    pcVar2 = pcVar6;
    pcVar6 = pcVar6 + 1;
    pcVar1 = pcVar5;
    pcVar5 = pcVar5 + 1;
    *pcVar2 = *pcVar1;
  }
  return;
}



/* ============================================================
   1000:16e6  FUN_1000_16e6
   ============================================================ */

int __cdecl16far FUN_1000_16e6(byte *param_1,char *param_2)

{
  byte *pbVar1;
  char *pcVar2;
  byte *pbVar3;
  int iVar4;
  uint uVar5;
  char *pcVar6;
  byte *pbVar7;
  char *pcVar8;
  byte *pbVar9;
  undefined2 uVar10;
  bool bVar11;
  bool bVar12;
  
  pbVar7 = (byte *)param_1;
  uVar10 = (undefined2)((ulong)param_2 >> 0x10);
  pcVar8 = (char *)param_2;
  iVar4 = 0;
  uVar5 = 0xffff;
  do {
    if (uVar5 == 0) break;
    uVar5 = uVar5 - 1;
    pcVar2 = pcVar8;
    pcVar8 = pcVar8 + 1;
  } while (*pcVar2 != '\0');
  pcVar6 = (char *)~uVar5;
  bVar11 = pcVar8 < pcVar6;
  pbVar9 = (byte *)(pcVar8 + -(int)pcVar6);
  bVar12 = pbVar9 == (byte *)0x0;
  do {
    if (pcVar6 == (char *)0x0) break;
    pcVar6 = pcVar6 + -1;
    pbVar3 = pbVar9;
    pbVar9 = pbVar9 + 1;
    pbVar1 = pbVar7;
    pbVar7 = pbVar7 + 1;
    bVar11 = *pbVar1 < *pbVar3;
    bVar12 = *pbVar1 == *pbVar3;
  } while (bVar12);
  if (!bVar12) {
    iVar4 = (1 - (uint)bVar11) - (uint)(bVar11 != 0);
  }
  return iVar4;
}



/* ============================================================
   1000:1710  FUN_1000_1710
   ============================================================ */

uint __cdecl16far FUN_1000_1710(char *param_1)

{
  char *pcVar1;
  uint uVar2;
  char *pcVar3;
  bool bVar4;
  
  pcVar3 = (char *)param_1;
  bVar4 = true;
  uVar2 = 0xffff;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar1 = pcVar3;
    pcVar3 = pcVar3 + 1;
    bVar4 = *pcVar1 == '\0';
  } while (!bVar4);
  uVar2 = ~uVar2;
  if (bVar4) {
    uVar2 = uVar2 - 1;
  }
  return uVar2;
}



/* ============================================================
   1000:172a  FUN_1000_172a
   ============================================================ */

char * __cdecl16far FUN_1000_172a(char *param_1,char *param_2,int param_3)

{
  char *pcVar1;
  char *pcVar2;
  int iVar3;
  char *pcVar4;
  char *pcVar5;
  char *pcVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  bool bVar9;
  
  uVar7 = (undefined2)((ulong)param_1 >> 0x10);
  iVar3 = -1;
  pcVar4 = (char *)param_1;
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    pcVar1 = pcVar4;
    pcVar4 = pcVar4 + 1;
  } while (*pcVar1 != '\0');
  pcVar4 = pcVar4 + -1;
  bVar9 = pcVar4 == (char *)0x0;
  uVar8 = (undefined2)((ulong)param_2 >> 0x10);
  pcVar5 = (char *)param_2;
  iVar3 = param_3;
  pcVar6 = pcVar5;
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    pcVar1 = pcVar6;
    pcVar6 = pcVar6 + 1;
    bVar9 = *pcVar1 == '\0';
  } while (!bVar9);
  if (bVar9) {
    iVar3 = iVar3 + 1;
  }
  for (iVar3 = -(iVar3 - param_3); iVar3 != 0; iVar3 = iVar3 + -1) {
    pcVar2 = pcVar4;
    pcVar4 = pcVar4 + 1;
    pcVar1 = pcVar5;
    pcVar5 = pcVar5 + 1;
    *pcVar2 = *pcVar1;
  }
  *pcVar4 = '\0';
  return (char *)param_1;
}



/* ============================================================
   1000:1762  FUN_1000_1762
   ============================================================ */

char * __cdecl16far FUN_1000_1762(char *param_1,char *param_2,int param_3)

{
  char *pcVar1;
  char *pcVar2;
  char *pcVar3;
  char *pcVar4;
  undefined2 uVar5;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  pcVar3 = (char *)param_2;
  pcVar4 = (char *)param_1;
  if (param_3 != 0) {
    do {
      pcVar1 = pcVar3;
      pcVar3 = pcVar3 + 1;
      if (*pcVar1 == '\0') break;
      pcVar2 = pcVar4;
      pcVar4 = pcVar4 + 1;
      *pcVar2 = *pcVar1;
      param_3 = param_3 + -1;
    } while (param_3 != 0);
    for (; param_3 != 0; param_3 = param_3 + -1) {
      pcVar1 = pcVar4;
      pcVar4 = pcVar4 + 1;
      *pcVar1 = '\0';
    }
  }
  return (char *)param_1;
}



/* ============================================================
   1000:178c  FUN_1000_178c
   ============================================================ */

uint __cdecl16far FUN_1000_178c(char *param_1,char *param_2,int param_3)

{
  char *pcVar1;
  char *pcVar2;
  int iVar3;
  uint uVar4;
  char *pcVar5;
  char *pcVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  
  uVar4 = 0;
  if (param_3 != 0) {
    uVar7 = (undefined2)((ulong)param_1 >> 0x10);
    pcVar6 = (char *)param_1;
    iVar3 = param_3;
    pcVar5 = pcVar6;
    do {
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      pcVar1 = pcVar5;
      pcVar5 = pcVar5 + 1;
    } while (*pcVar1 != '\0');
    param_3 = param_3 - iVar3;
    uVar8 = (undefined2)((ulong)param_2 >> 0x10);
    pcVar5 = (char *)param_2;
    do {
      if (param_3 == 0) break;
      param_3 = param_3 + -1;
      pcVar2 = pcVar6;
      pcVar6 = pcVar6 + 1;
      pcVar1 = pcVar5;
      pcVar5 = pcVar5 + 1;
    } while (*pcVar1 == *pcVar2);
    uVar4 = 0;
    if ((byte)pcVar5[-1] <= (byte)pcVar6[-1]) {
      if (pcVar5[-1] == pcVar6[-1]) {
        return 0;
      }
      uVar4 = 0xfffe;
    }
    uVar4 = ~uVar4;
  }
  return uVar4;
}



/* ============================================================
   1000:17c8  thunk_FUN_1000_17cc
   ============================================================ */

int __cdecl16far thunk_FUN_1000_17cc(byte *param_1)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  byte *pbVar5;
  undefined2 uVar6;
  
  uVar6 = (undefined2)((ulong)param_1 >> 0x10);
  pbVar5 = (byte *)param_1;
  iVar4 = 0;
  do {
    do {
      pbVar1 = pbVar5;
      pbVar5 = pbVar5 + 1;
      bVar2 = *pbVar1;
    } while (bVar2 == 0x20);
  } while (bVar2 == 9);
  if ((bVar2 != 0x2d) && (bVar3 = bVar2, bVar2 != 0x2b)) goto LAB_1000_17ed;
  while( true ) {
    pbVar1 = pbVar5;
    pbVar5 = pbVar5 + 1;
    bVar3 = *pbVar1;
LAB_1000_17ed:
    if ((0x39 < bVar3) || (bVar3 < 0x30)) break;
    iVar4 = iVar4 * 10 + (uint)(byte)(bVar3 - 0x30);
  }
  if (bVar2 == 0x2d) {
    iVar4 = -iVar4;
  }
  return iVar4;
}



/* ============================================================
   1000:17cc  FUN_1000_17cc
   ============================================================ */

int __cdecl16far FUN_1000_17cc(byte *param_1)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  byte *pbVar5;
  undefined2 uVar6;
  
  uVar6 = (undefined2)((ulong)param_1 >> 0x10);
  pbVar5 = (byte *)param_1;
  iVar4 = 0;
  do {
    do {
      pbVar1 = pbVar5;
      pbVar5 = pbVar5 + 1;
      bVar2 = *pbVar1;
    } while (bVar2 == 0x20);
  } while (bVar2 == 9);
  if ((bVar2 != 0x2d) && (bVar3 = bVar2, bVar2 != 0x2b)) goto LAB_1000_17ed;
  while( true ) {
    pbVar1 = pbVar5;
    pbVar5 = pbVar5 + 1;
    bVar3 = *pbVar1;
LAB_1000_17ed:
    if ((0x39 < bVar3) || (bVar3 < 0x30)) break;
    iVar4 = iVar4 * 10 + (uint)(byte)(bVar3 - 0x30);
  }
  if (bVar2 == 0x2d) {
    iVar4 = -iVar4;
  }
  return iVar4;
}



/* ============================================================
   1000:1822  FUN_1000_1822
   ============================================================ */

void FUN_1000_1822(void)

{
  FUN_1000_1eeb();
  return;
}



/* ============================================================
   1000:183e  FUN_1000_183e
   ============================================================ */

void FUN_1000_183e(void)

{
  FUN_1000_1ede();
  return;
}



/* ============================================================
   1000:1848  FUN_1000_1848
   ============================================================ */

int __cdecl16far FUN_1000_1848(int param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  undefined2 uVar5;
  undefined4 local_8;
  
  local_8 = (int *)CONCAT22(DAT_1080_24f4,DAT_1080_24f2);
  if ((DAT_1080_24f4 != 0 || DAT_1080_24f2 != (int *)0x0) && (param_2 != 0 || param_1 != 0)) {
    uVar1 = FUN_1000_1710(param_1,param_2);
    while( true ) {
      uVar5 = (undefined2)((ulong)local_8 >> 0x10);
      piVar4 = (int *)local_8;
      if (piVar4[1] == 0 && *local_8 == 0) break;
      uVar2 = FUN_1000_1710(*local_8,piVar4[1]);
      if (((uVar1 < uVar2) && (*(char *)((int)*(undefined4 *)local_8 + uVar1) == '=')) &&
         (iVar3 = FUN_1000_178c(*local_8,piVar4[1],param_1,param_2,uVar1), iVar3 == 0)) {
        return *local_8 + uVar1 + 1;
      }
      local_8 = (int *)CONCAT22(uVar5,piVar4 + 2);
    }
  }
  return 0;
}



/* ============================================================
   1000:18e4  FUN_1000_18e4
   ============================================================ */

int __cdecl16far FUN_1000_18e4(void)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = 0;
  if (DAT_1080_2612 == 0) {
    uVar2 = 0x2626;
  }
  else {
    uVar2 = 0x264a;
  }
  for (; uVar2 <= DAT_1080_2524; uVar2 = uVar2 + 0xc) {
    iVar1 = FUN_1000_071a(uVar2,0x1080);
    if (iVar1 != -1) {
      iVar3 = iVar3 + 1;
    }
  }
  return iVar3;
}



/* ============================================================
   1000:1924  FUN_1000_1924
   ============================================================ */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 __cdecl16far
FUN_1000_1924(undefined1 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  undefined1 *puVar1;
  undefined2 uVar2;
  
  DAT_1080_2a34 = 0x42;
  DAT_1080_2a30 = param_1;
  DAT_1080_2a32 = param_2;
  _DAT_1080_2a2a = (undefined1 *)CONCAT22(param_2,param_1);
  DAT_1080_2a2e = 0x7fff;
  uVar2 = FUN_1000_0b1c((undefined2 *)&DAT_1080_2a2a,0x1080,param_3,param_4,&stack0x000c);
  puVar1 = _DAT_1080_2a2a;
  DAT_1080_2a2e = DAT_1080_2a2e + -1;
  if (DAT_1080_2a2e < 0) {
    FUN_1000_0832(0,(undefined2 *)&DAT_1080_2a2a,0x1080);
  }
  else {
    _DAT_1080_2a2a = (undefined1 *)CONCAT22(_DAT_1080_2a2c,DAT_1080_2a2a + 1);
    *puVar1 = 0;
  }
  return uVar2;
}



/* ============================================================
   1000:1992  FUN_1000_1992
   ============================================================ */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 __cdecl16far FUN_1000_1992(int param_1)

{
  undefined2 uVar1;
  int iVar2;
  
  if ((param_1 < 0) || (DAT_1080_24c2 <= param_1)) {
    DAT_1080_24ac = 9;
    uVar1 = 0xffff;
  }
  else if (((DAT_1080_2612 == 0) || ((param_1 < DAT_1080_24be && (2 < param_1)))) &&
          (0x31d < CONCAT11(DAT_1080_24b7,DAT_1080_24b6))) {
    iVar2 = _DAT_1080_24bc;
    if (((*(byte *)(param_1 + 0x24c4) & 1) != 0) && (iVar2 = FUN_1000_1fce(), iVar2 == 0)) {
      return 0;
    }
    _DAT_1080_24bc = iVar2;
    DAT_1080_24ac = 9;
    uVar1 = 0xffff;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



/* ============================================================
   1000:1a02  FUN_1000_1a02
   ============================================================ */

void __cdecl16far FUN_1000_1a02(undefined2 *param_1)

{
  code *pcVar1;
  undefined2 uVar2;
  uint in_CX;
  uint uVar3;
  uint extraout_DX;
  uint extraout_DX_00;
  uint uVar4;
  undefined1 extraout_DH;
  undefined1 extraout_DH_00;
  undefined1 uVar6;
  uint extraout_DX_01;
  uint extraout_DX_02;
  uint uVar5;
  int unaff_BP;
  char in_stack_0000ffec;
  
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)(0x1080,unaff_BP + 1);
    uVar4 = extraout_DX_00;
  }
  else {
    DOS3CALL(0x1000);
    uVar4 = extraout_DX;
  }
  uVar3 = in_CX;
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
    uVar6 = extraout_DH_00;
  }
  else {
    DOS3CALL(0x1000);
    uVar6 = extraout_DH;
  }
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)(uVar3 >> 8,uVar3 & 0xff,uVar6);
    uVar5 = extraout_DX_02;
  }
  else {
    in_stack_0000ffec = '\0';
    DOS3CALL(0x1000,uVar3 >> 8);
    uVar5 = extraout_DX_01;
  }
  if ((uVar4 != uVar5) && (in_stack_0000ffec == '\x17')) {
    uVar3 = in_CX;
    uVar5 = uVar4;
  }
  uVar2 = FUN_1000_1cd4(uVar3 - 0x7bc,uVar5 >> 8,uVar5 & 0xff);
  if (param_1._2_2_ != 0) {
    ((undefined2 *)param_1)[1] = uVar5;
    *param_1 = uVar2;
  }
  return;
}



/* ============================================================
   1000:1a96  FUN_1000_1a96
   ============================================================ */

void __cdecl16far FUN_1000_1a96(void)

{
  int unaff_BP;
  
  if (DAT_1080_2a36 == 0) {
    FUN_1000_1ab2(0x1080,unaff_BP + 1);
    DAT_1080_2a36 = DAT_1080_2a36 + 1;
  }
  return;
}



/* ============================================================
   1000:1ab2  FUN_1000_1ab2
   ============================================================ */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl16far FUN_1000_1ab2(void)

{
  char cVar1;
  long lVar2;
  char *pcVar3;
  undefined2 uVar4;
  uint uVar5;
  int in_DX;
  undefined2 uVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  undefined2 uVar10;
  undefined2 uVar11;
  undefined4 local_8;
  
  pcVar3 = (char *)FUN_1000_1848(0x25f4,0x1080);
  local_8 = (char *)CONCAT22(in_DX,pcVar3);
  if ((in_DX != 0 || pcVar3 != (char *)0x0) && (*local_8 != '\0')) {
    FUN_1000_1762(DAT_1080_2606,DAT_1080_2608,pcVar3,in_DX,3);
    local_8 = (char *)CONCAT22(in_DX,pcVar3 + 3);
    cVar1 = *local_8;
    if (cVar1 == '-') {
      local_8 = (char *)CONCAT22(in_DX,pcVar3 + 4);
    }
    uVar6 = 0;
    uVar11 = 0;
    uVar10 = 0xe10;
    uVar4 = thunk_FUN_1000_17cc((char *)local_8,in_DX,0xe10,0);
    lVar8 = FUN_1000_1ff2(uVar4,uVar6,uVar10,uVar11);
    DAT_1080_25fa = (int)((ulong)lVar8 >> 0x10);
    DAT_1080_25f8 = (uint)lVar8;
    while( true ) {
      uVar4 = (undefined2)((ulong)local_8 >> 0x10);
      if ((*local_8 != '+') && ((*local_8 < '0' || ('9' < *local_8)))) break;
      local_8 = (char *)CONCAT22(uVar4,(char *)local_8 + 1);
    }
    if (*local_8 == ':') {
      uVar6 = 0;
      uVar11 = 0;
      uVar10 = 0x3c;
      local_8._0_2_ = (char *)local_8 + 1;
      local_8 = (char *)CONCAT22(uVar4,(char *)local_8);
      uVar4 = thunk_FUN_1000_17cc((char *)local_8,uVar4,0x3c,0);
      lVar9 = FUN_1000_1ff2(uVar4,uVar6,uVar10,uVar11);
      lVar8 = CONCAT22(DAT_1080_25fa,DAT_1080_25f8);
      iVar7 = (int)((ulong)lVar9 >> 0x10);
      lVar2 = lVar9 + lVar8;
      DAT_1080_25f8 = (uint)lVar2;
      DAT_1080_25fa = (int)((ulong)lVar2 >> 0x10);
      while( true ) {
        uVar4 = (undefined2)((ulong)local_8 >> 0x10);
        if ((*local_8 < '0') || ('9' < *local_8)) break;
        local_8 = (char *)CONCAT22(uVar4,(char *)local_8 + 1);
      }
      lVar8 = lVar9 + lVar8;
      if (*local_8 == ':') {
        local_8._0_2_ = (char *)local_8 + 1;
        local_8 = (char *)CONCAT22(uVar4,(char *)local_8);
        uVar5 = thunk_FUN_1000_17cc((char *)local_8,uVar4);
        iVar7 = DAT_1080_25fa + iVar7 + (uint)CARRY2(DAT_1080_25f8,uVar5);
        while ((lVar8 = CONCAT22(iVar7,DAT_1080_25f8 + uVar5), '/' < *local_8 &&
               (lVar8 = CONCAT22(iVar7,DAT_1080_25f8 + uVar5), *local_8 < ':'))) {
          local_8 = (char *)CONCAT22(local_8._2_2_,(char *)local_8 + 1);
        }
      }
    }
    DAT_1080_25fa = (int)((ulong)lVar8 >> 0x10);
    DAT_1080_25f8 = (uint)lVar8;
    if (cVar1 == '-') {
      lVar8 = CONCAT22(-(DAT_1080_25fa + (uint)(DAT_1080_25f8 != 0)),-DAT_1080_25f8);
    }
    DAT_1080_25fa = (int)((ulong)lVar8 >> 0x10);
    DAT_1080_25f8 = (uint)lVar8;
    DAT_1080_25fc = (int)*local_8;
    if (DAT_1080_25fc == 0) {
      *_DAT_1080_260a = 0;
    }
    else {
      FUN_1000_1762(DAT_1080_260a,DAT_1080_260c,(char *)local_8,(int)((ulong)local_8 >> 0x10),3);
    }
  }
  return;
}



/* ============================================================
   1000:1c00  FUN_1000_1c00
   ============================================================ */

undefined2 __cdecl16far FUN_1000_1c00(undefined4 param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  int local_6;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  if (((*(int *)(iVar4 + 10) < 0x43) || (*(int *)(iVar4 + 8) < 3)) || (9 < *(int *)(iVar4 + 8)))
  goto LAB_1000_1cc9;
  if ((*(int *)(iVar4 + 8) < 4) || (8 < *(int *)(iVar4 + 8))) {
    uVar1 = *(uint *)(iVar4 + 10);
    if (((int)uVar1 < 0x57) || (*(int *)(iVar4 + 8) != 3)) {
      local_6 = *(int *)(*(int *)(iVar4 + 8) * 2 + 0x25dc);
    }
    else {
      local_6 = *(int *)(*(int *)(iVar4 + 8) * 2 + 0x25da) + 7;
    }
    if ((uVar1 & 3) == 0) {
      local_6 = local_6 + 1;
    }
    uVar1 = (uVar1 - 0x46) * 0x16d + ((int)(uVar1 - 1) >> 2) + local_6;
    iVar2 = FUN_1000_2024(uVar1 - 0xd,((int)uVar1 >> 0xf) - (uint)(uVar1 < 0xd),7,0);
    iVar3 = -(iVar2 - local_6);
    if (*(int *)(iVar4 + 8) == 3) {
      if ((iVar3 < *(int *)(iVar4 + 0xe)) ||
         ((-*(int *)(iVar4 + 0xe) == iVar2 - local_6 && (1 < *(int *)(iVar4 + 4)))))
      goto LAB_1000_1cb4;
    }
    else if ((*(int *)(iVar4 + 0xe) < iVar3) ||
            ((*(int *)(iVar4 + 0xe) == iVar3 && (*(int *)(iVar4 + 4) < 1)))) goto LAB_1000_1cb4;
LAB_1000_1cc9:
    uVar5 = 0;
  }
  else {
LAB_1000_1cb4:
    uVar5 = 1;
  }
  return uVar5;
}



/* ============================================================
   1000:1cd4  FUN_1000_1cd4
   ============================================================ */

int __cdecl16far
FUN_1000_1cd4(uint param_1,int param_2,uint param_3,uint param_4,uint param_5,int param_6)

{
  uint uVar1;
  int iVar2;
  int unaff_BP;
  int iVar3;
  undefined4 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  int local_1a;
  undefined1 local_16 [4];
  uint local_12;
  int local_e;
  int local_c;
  int local_8;
  undefined2 local_4;
  int iStack_2;
  
  iStack_2 = unaff_BP + 1;
  local_4 = 0x1080;
  iVar3 = *(int *)(param_2 * 2 + 0x25d8);
  if (((param_1 & 3) == 0) && (2 < param_2)) {
    iVar3 = iVar3 + 1;
  }
  FUN_1000_1a96();
  uVar8 = 0;
  uVar7 = 0x3c;
  uVar6 = 0;
  uVar5 = 0x3c;
  uVar1 = (int)(param_1 + 3) >> 2;
  uVar4 = FUN_1000_1ff2((long)(int)param_1 * 0x16d +
                        CONCAT22(((int)(param_1 + 3) >> 0xf) + ((int)param_3 >> 0xf) +
                                 (uint)CARRY2(uVar1,param_3),uVar1 + param_3) + (long)iVar3 + 0xe44,
                        0x18,0);
  uVar4 = FUN_1000_1ff2((uint)uVar4 + param_4,
                        (int)((ulong)uVar4 >> 0x10) + ((int)param_4 >> 0xf) +
                        (uint)CARRY2((uint)uVar4,param_4),uVar5,uVar6);
  iVar2 = FUN_1000_1ff2((uint)uVar4 + param_5,
                        (int)((ulong)uVar4 >> 0x10) + ((int)param_5 >> 0xf) +
                        (uint)CARRY2((uint)uVar4,param_5),uVar7,uVar8);
  local_1a = iVar2 + param_6 + DAT_1080_25f8;
  local_8 = param_3 + iVar3;
  local_c = param_1 + 0x50;
  local_e = param_2 + -1;
  local_12 = param_4;
  if (DAT_1080_25fc != 0) {
    iVar3 = FUN_1000_1c00(local_16);
    if (iVar3 != 0) {
      local_1a = local_1a + -0xe10;
    }
  }
  return local_1a;
}



/* ============================================================
   1000:1dd2  FUN_1000_1dd2
   ============================================================ */

char * __cdecl16far FUN_1000_1dd2(char *param_1,char param_2)

{
  char *pcVar1;
  uint uVar2;
  char *pcVar3;
  char *pcVar4;
  undefined2 uVar5;
  bool bVar6;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  pcVar3 = (char *)param_1;
  bVar6 = true;
  uVar2 = 0xffff;
  pcVar4 = pcVar3;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar1 = pcVar4;
    pcVar4 = pcVar4 + 1;
    bVar6 = *pcVar1 == '\0';
  } while (!bVar6);
  uVar2 = ~uVar2;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar1 = pcVar3;
    pcVar3 = pcVar3 + 1;
    bVar6 = param_2 == *pcVar1;
  } while (!bVar6);
  if (!bVar6) {
    if (param_2 != '\0') {
      return (char *)0x0;
    }
    pcVar3 = pcVar3 + 1;
  }
  return pcVar3 + -1;
}



/* ============================================================
   1000:1e04  FUN_1000_1e04
   ============================================================ */

char * __cdecl16far FUN_1000_1e04(char *param_1,char param_2)

{
  char *pcVar1;
  uint uVar2;
  char *pcVar3;
  char *pcVar4;
  undefined2 uVar5;
  bool bVar6;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  pcVar3 = (char *)param_1;
  uVar2 = 0xffff;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar1 = pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (*pcVar1 != '\0');
  uVar2 = ~uVar2;
  pcVar4 = pcVar3 + -1;
  bVar6 = param_2 == '\0';
  if (bVar6) {
    if (*pcVar4 == '\0') {
      pcVar4 = pcVar3 + -2;
    }
  }
  else {
    do {
      if (uVar2 == 0) break;
      uVar2 = uVar2 - 1;
      pcVar1 = pcVar4;
      pcVar4 = pcVar4 + -1;
      bVar6 = param_2 == *pcVar1;
    } while (!bVar6);
    if (!bVar6) {
      return (char *)0x0;
    }
  }
  return pcVar4 + 1;
}



/* ============================================================
   1000:1e38  FUN_1000_1e38
   ============================================================ */

undefined2 * __cdecl16far FUN_1000_1e38(undefined2 *param_1,undefined2 *param_2,int param_3)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  uint uVar3;
  uint uVar4;
  undefined2 *puVar5;
  undefined2 *puVar6;
  int iVar7;
  int iVar8;
  
  if (param_3 != 0) {
    iVar8 = (int)((ulong)param_2 >> 0x10);
    puVar5 = (undefined2 *)param_2;
    iVar7 = (int)((ulong)param_1 >> 0x10);
    puVar6 = (undefined2 *)param_1;
    while( true ) {
      uVar3 = ~(uint)puVar6;
      uVar3 = ((param_3 - 1U) - uVar3 & -(uint)(param_3 - 1U < uVar3)) + uVar3;
      uVar4 = ~(uint)puVar5;
      uVar3 = (uVar3 - uVar4 & -(uint)(uVar3 < uVar4)) + uVar4 + 1;
      param_3 = param_3 - uVar3;
      for (uVar4 = uVar3 >> 1; uVar4 != 0; uVar4 = uVar4 - 1) {
        puVar2 = puVar6;
        puVar6 = puVar6 + 1;
        puVar1 = puVar5;
        puVar5 = puVar5 + 1;
        *puVar2 = *puVar1;
      }
      for (uVar3 = (uint)((uVar3 & 1) != 0); uVar3 != 0; uVar3 = uVar3 - 1) {
        puVar2 = puVar6;
        puVar6 = (undefined2 *)((int)puVar6 + 1);
        puVar1 = puVar5;
        puVar5 = (undefined2 *)((int)puVar5 + 1);
        *(undefined1 *)puVar2 = *(undefined1 *)puVar1;
      }
      if (param_3 == 0) break;
      if (puVar5 == (undefined2 *)0x0) {
        iVar8 = iVar8 + 0x5c;
      }
      if (puVar6 == (undefined2 *)0x0) {
        iVar7 = iVar7 + 0x5c;
      }
    }
  }
  return (undefined2 *)param_1;
}



/* ============================================================
   1000:1e96  FUN_1000_1e96
   ============================================================ */

undefined2 * __cdecl16far FUN_1000_1e96(undefined2 *param_1,undefined1 param_2,uint param_3)

{
  undefined2 *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  undefined2 *puVar5;
  int iVar6;
  
  if (param_3 != 0) {
    iVar6 = (int)((ulong)param_1 >> 0x10);
    uVar2 = -(int)(undefined2 *)param_1;
    uVar4 = 0;
    uVar3 = param_3;
    if (uVar2 != 0) {
      uVar3 = (uVar2 - param_3 & -(uint)(uVar2 < param_3)) + param_3;
      uVar4 = param_3 - uVar3;
    }
    puVar5 = (undefined2 *)param_1;
    for (uVar2 = uVar3 >> 1; uVar2 != 0; uVar2 = uVar2 - 1) {
      puVar1 = puVar5;
      puVar5 = puVar5 + 1;
      *puVar1 = CONCAT11(param_2,param_2);
    }
    for (uVar3 = (uint)((uVar3 & 1) != 0); uVar3 != 0; uVar3 = uVar3 - 1) {
      puVar1 = puVar5;
      puVar5 = (undefined2 *)((int)puVar5 + 1);
      *(undefined1 *)puVar1 = param_2;
    }
    if (uVar4 != 0) {
      for (uVar3 = uVar4 >> 1; uVar3 != 0; uVar3 = uVar3 - 1) {
        puVar1 = puVar5;
        puVar5 = puVar5 + 1;
        *puVar1 = CONCAT11(param_2,param_2);
      }
      for (uVar4 = (uint)((uVar4 & 1) != 0); uVar4 != 0; uVar4 = uVar4 - 1) {
        puVar1 = puVar5;
        puVar5 = (undefined2 *)((int)puVar5 + 1);
        *(undefined1 *)puVar1 = param_2;
      }
    }
  }
  return (undefined2 *)param_1;
}



/* ============================================================
   1000:1ede  FUN_1000_1ede
   ============================================================ */

byte * FUN_1000_1ede(void)

{
  byte *pbVar1;
  byte *pbVar2;
  uint uVar3;
  ulong uVar4;
  byte *pbVar5;
  byte bVar6;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  char in_BL;
  int iVar11;
  int unaff_BP;
  byte *pbVar12;
  byte *pbVar13;
  undefined2 unaff_SS;
  undefined2 uVar14;
  bool bVar15;
  char cVar7;
  
  uVar3 = *(uint *)(unaff_BP + 0xe);
  iVar11 = *(int *)(unaff_BP + 6);
  uVar9 = *(uint *)(unaff_BP + 8);
  pbVar5 = (byte *)*(undefined4 *)(unaff_BP + 10);
  uVar14 = (undefined2)((ulong)pbVar5 >> 0x10);
  pbVar13 = (byte *)pbVar5;
  pbVar12 = pbVar13;
  if (((in_BL != '\0') && (uVar3 == 10)) && ((int)uVar9 < 0)) {
    pbVar13 = pbVar13 + 1;
    *pbVar5 = 0x2d;
    bVar15 = iVar11 != 0;
    iVar11 = -iVar11;
    uVar9 = -(uVar9 + bVar15);
    pbVar12 = pbVar13;
  }
  do {
    uVar10 = 0;
    uVar8 = uVar9;
    if (uVar9 != 0) {
      uVar8 = uVar9 / uVar3;
      uVar10 = uVar9 % uVar3;
    }
    uVar4 = CONCAT22(uVar10,iVar11);
    iVar11 = (int)(uVar4 / uVar3);
    cVar7 = (char)(uVar4 % (ulong)uVar3);
    bVar6 = cVar7 + 0x30;
    if (0x39 < bVar6) {
      bVar6 = cVar7 + 0x57;
    }
    pbVar2 = pbVar13;
    pbVar13 = pbVar13 + 1;
    *pbVar2 = bVar6;
    uVar9 = uVar8;
  } while (uVar8 != 0 || iVar11 != 0);
  *pbVar13 = 0;
  do {
    pbVar13 = pbVar13 + -1;
    LOCK();
    bVar6 = *pbVar13;
    *pbVar13 = *pbVar12;
    UNLOCK();
    *pbVar12 = bVar6;
    pbVar1 = pbVar12 + 2;
    pbVar12 = pbVar12 + 1;
  } while (pbVar1 < pbVar13);
  return pbVar5;
}



/* ============================================================
   1000:1eeb  FUN_1000_1eeb
   ============================================================ */

byte * __cdecl16far FUN_1000_1eeb(void)

{
  ulong uVar1;
  byte bVar2;
  int in_AX;
  uint uVar4;
  uint in_CX;
  uint in_DX;
  uint uVar5;
  char in_BL;
  byte *pbVar6;
  byte *unaff_DI;
  byte *pbVar7;
  byte *pbVar8;
  bool bVar9;
  char cVar3;
  
  pbVar7 = unaff_DI;
  pbVar6 = unaff_DI;
  if (((in_BL != '\0') && (in_CX == 10)) && ((int)in_DX < 0)) {
    pbVar7 = unaff_DI + 1;
    *unaff_DI = 0x2d;
    bVar9 = in_AX != 0;
    in_AX = -in_AX;
    in_DX = -(in_DX + bVar9);
    pbVar6 = pbVar7;
  }
  do {
    uVar5 = 0;
    uVar4 = in_DX;
    if (in_DX != 0) {
      uVar4 = in_DX / in_CX;
      uVar5 = in_DX % in_CX;
    }
    uVar1 = CONCAT22(uVar5,in_AX);
    in_AX = (int)(uVar1 / in_CX);
    cVar3 = (char)(uVar1 % (ulong)in_CX);
    bVar2 = cVar3 + 0x30;
    if (0x39 < bVar2) {
      bVar2 = cVar3 + 0x57;
    }
    pbVar8 = pbVar7 + 1;
    *pbVar7 = bVar2;
    in_DX = uVar4;
    pbVar7 = pbVar8;
  } while (uVar4 != 0 || in_AX != 0);
  *pbVar8 = 0;
  do {
    pbVar8 = pbVar8 + -1;
    LOCK();
    bVar2 = *pbVar8;
    *pbVar8 = *pbVar6;
    UNLOCK();
    *pbVar6 = bVar2;
    pbVar7 = pbVar6 + 2;
    pbVar6 = pbVar6 + 1;
  } while (pbVar7 < pbVar8);
  return unaff_DI;
}



/* ============================================================
   1000:1f42  FUN_1000_1f42
   ============================================================ */

int __cdecl16far FUN_1000_1f42(uint param_1)

{
  return (param_1 ^ (int)param_1 >> 0xf) - ((int)param_1 >> 0xf);
}



/* ============================================================
   1000:1f56  FUN_1000_1f56
   ============================================================ */

void __cdecl16far FUN_1000_1f56(undefined2 param_1)

{
  DAT_1080_260e = param_1;
  DAT_1080_2610 = 0;
  return;
}



/* ============================================================
   1000:1f6e  FUN_1000_1f6e
   ============================================================ */

uint __cdecl16far FUN_1000_1f6e(void)

{
  long lVar1;
  
  lVar1 = FUN_1000_1ff2(DAT_1080_260e,DAT_1080_2610,0x43fd,3);
  DAT_1080_2610 = (uint)((ulong)(lVar1 + 0x269ec3) >> 0x10);
  DAT_1080_260e = (int)(lVar1 + 0x269ec3);
  return DAT_1080_2610 & 0x7fff;
}



/* ============================================================
   1000:1fa8  FUN_1000_1fa8
   ============================================================ */

void FUN_1000_1fa8(void)

{
  code *pcVar1;
  int unaff_BP;
  
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)(0x1080,unaff_BP + 1);
  }
  else {
    DOS3CALL(0x1000,0x1080);
  }
  FUN_1000_0698();
  return;
}



/* ============================================================
   1000:1fce  FUN_1000_1fce
   ============================================================ */

void FUN_1000_1fce(void)

{
  code *pcVar1;
  int unaff_BP;
  
  if ((DAT_1000_0010 & 1) == 0) {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)(unaff_BP + 1);
  }
  else {
    DOS3CALL(0x1000,0x1080);
  }
  FUN_1000_06a5();
  return;
}



/* ============================================================
   1000:1ff2  FUN_1000_1ff2
   ============================================================ */

long __stdcall16far FUN_1000_1ff2(uint param_1,int param_2,uint param_3,int param_4)

{
  if (param_4 == 0 && param_2 == 0) {
    return (ulong)param_1 * (ulong)param_3;
  }
  return CONCAT22((int)((ulong)param_1 * (ulong)param_3 >> 0x10) +
                  param_2 * param_3 + param_1 * param_4,(int)((ulong)param_1 * (ulong)param_3));
}



/* ============================================================
   1000:2024  FUN_1000_2024
   ============================================================ */

undefined4 __stdcall16far FUN_1000_2024(uint param_1,uint param_2,uint param_3,uint param_4)

{
  ulong uVar1;
  long lVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  bool bVar12;
  bool bVar13;
  
  bVar13 = (int)param_2 < 0;
  if (bVar13) {
    bVar12 = param_1 != 0;
    param_1 = -param_1;
    param_2 = -(uint)bVar12 - param_2;
  }
  uVar11 = (uint)bVar13;
  if ((int)param_4 < 0) {
    bVar13 = param_3 != 0;
    param_3 = -param_3;
    param_4 = -(uint)bVar13 - param_4;
  }
  uVar3 = param_1;
  uVar4 = param_3;
  uVar8 = param_2;
  uVar9 = param_4;
  if (param_4 == 0) {
    iVar5 = (int)(((ulong)param_2 % (ulong)param_3 << 0x10 | (ulong)param_1) % (ulong)param_3);
    iVar6 = 0;
    if ((int)(uVar11 - 1) < 0) goto LAB_1000_20be;
  }
  else {
    do {
      uVar10 = uVar9 >> 1;
      uVar4 = uVar4 >> 1 | (uint)((uVar9 & 1) != 0) << 0xf;
      uVar7 = uVar8 >> 1;
      uVar3 = uVar3 >> 1 | (uint)((uVar8 & 1) != 0) << 0xf;
      uVar8 = uVar7;
      uVar9 = uVar10;
    } while (uVar10 != 0);
    uVar1 = CONCAT22(uVar7,uVar3) / (ulong)uVar4;
    uVar3 = (int)uVar1 * param_4;
    lVar2 = (uVar1 & 0xffff) * (ulong)param_3;
    uVar8 = (uint)((ulong)lVar2 >> 0x10);
    uVar4 = (uint)lVar2;
    uVar9 = uVar8 + uVar3;
    if (((CARRY2(uVar8,uVar3)) || (param_2 < uVar9)) || ((param_2 <= uVar9 && (param_1 < uVar4)))) {
      bVar13 = uVar4 < param_3;
      uVar4 = uVar4 - param_3;
      uVar9 = (uVar9 - param_4) - (uint)bVar13;
    }
    iVar5 = uVar4 - param_1;
    iVar6 = (uVar9 - param_2) - (uint)(uVar4 < param_1);
    if (-1 < (int)(uVar11 - 1)) goto LAB_1000_20be;
  }
  bVar13 = iVar5 != 0;
  iVar5 = -iVar5;
  iVar6 = -(uint)bVar13 - iVar6;
LAB_1000_20be:
  return CONCAT22(iVar6,iVar5);
}



/* ============================================================
   1000:20c4  FUN_1000_20c4
   ============================================================ */

void __cdecl16near FUN_1000_20c4(void)

{
  return;
}



/* ============================================================
   1000:20c9  FUN_1000_20c9
   ============================================================ */

void FUN_1000_20c9(void)

{
  FUN_1000_05b4();
  return;
}



/* ============================================================
   1008:0000  FUN_1008_0000
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_0000(undefined2 param_1,undefined2 param_2,undefined2 param_3,int param_4,
             undefined2 param_5)

{
  undefined2 uVar1;
  undefined2 unaff_SS;
  long lVar2;
  undefined1 local_35a [560];
  undefined1 local_12a [296];
  
                    /* Segment:    2
                       Offset:     00002b00
                       Length:     eff9
                       Min Alloc:  eff9
                       Flags:      1d50
                           Code
                           Discardable
                           Moveable
                           Preload
                           Impure (Non-shareable)
                        */
  DAT_1080_2988 = param_5;
  if (param_4 == 0) {
    FUN_1008_0258(local_35a,unaff_SS,param_1,param_5);
    FUN_1008_051a(local_35a,unaff_SS);
    FUN_1008_169c(local_35a,unaff_SS);
    FUN_1008_1abc(local_35a,unaff_SS);
    FUN_1008_198e(local_35a,unaff_SS);
    FUN_1010_edea(local_12a,unaff_SS,local_35a,unaff_SS);
    lVar2 = FUN_1018_01aa(local_12a,unaff_SS);
    if (lVar2 == 1) {
      FUN_1008_2d02(local_35a,unaff_SS,0,0xc4,(char *)s_Lucky_Chip_1080_1004 + 4);
      FUN_1008_2dfc(local_35a,unaff_SS);
      FUN_1008_a5fa();
    }
    uVar1 = FUN_1008_1aec(local_35a,unaff_SS);
    FUN_1010_ef2e(local_12a,unaff_SS);
    FUN_1008_0344(local_35a,unaff_SS);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



/* ============================================================
   1008:0258  FUN_1008_0258
   ============================================================ */

undefined2 * __stdcall16far
FUN_1008_0258(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  int iVar2;
  int extraout_DX;
  undefined4 uVar3;
  undefined2 uVar4;
  
  *param_1 = param_4;
  FUN_1008_4222(param_4,1);
  FUN_1008_4222(param_4,2);
  param_1[1] = param_3;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[9] = 0;
  param_1[8] = 0;
  param_1[7] = 0;
  param_1[6] = 0;
  param_1[2] = 0;
  param_1[0x6a] = 0;
  param_1[0x69] = 0;
  param_1[0x70] = 1;
  uVar4 = 0xffee;
  param_1[0x71] = 0;
  iVar1 = CREATEFONT((char *)s_Lucky_Chip_1080_1004 + 4,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,
                     0x31,0,0,5,0,0,0,0,700,0,0,0);
  param_1[0x6b] = iVar1;
  if (iVar1 == 0) {
    FUN_1008_2d02(param_1,param_2,0,0x32e4,(char *)s_Lucky_Chip_1080_1004 + 4);
  }
  FUN_1008_2eba(param_1,param_2);
  iVar1 = extraout_DX;
  iVar2 = FUN_1000_1646(0x26,uVar4);
  if (iVar1 == 0 && iVar2 == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = FUN_1010_ed68(iVar2,iVar1);
  }
  param_1[0x67] = (int)uVar3;
  param_1[0x68] = (int)((ulong)uVar3 >> 0x10);
  param_1[3] = 0;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1008:0344  FUN_1008_0344
   ============================================================ */

void __stdcall16far FUN_1008_0344(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  char *pcVar5;
  char *pcVar6;
  
  uVar4 = (undefined2)((ulong)param_1 >> 0x10);
  iVar3 = (int)param_1;
  pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
  FUN_1008_045e(iVar3,uVar4,*(undefined2 *)(iVar3 + 0xd8),*(undefined2 *)(iVar3 + 0xda));
  if (*(int *)(iVar3 + 10) != 0 || *(int *)(iVar3 + 8) != 0) {
    iVar1 = *(int *)(iVar3 + 8);
    iVar2 = *(int *)(iVar3 + 10);
    if (iVar2 != 0 || iVar1 != 0) {
      FUN_1008_6cd8(iVar1,iVar2);
      pcVar5 = (char *)0x1000;
      FUN_1000_1634(iVar1,iVar2);
    }
  }
  if (*(int *)(iVar3 + 0x12) != 0 || *(int *)(iVar3 + 0x10) != 0) {
    iVar1 = *(int *)(iVar3 + 0x10);
    iVar2 = *(int *)(iVar3 + 0x12);
    if (iVar2 != 0 || iVar1 != 0) {
      FUN_1008_46ce(iVar1,iVar2);
      pcVar5 = (char *)0x1000;
      FUN_1000_1634(iVar1,iVar2);
    }
  }
  if (*(int *)(iVar3 + 0xe) != 0 || *(int *)(iVar3 + 0xc) != 0) {
    iVar1 = *(int *)(iVar3 + 0xc);
    iVar2 = *(int *)(iVar3 + 0xe);
    if (iVar2 != 0 || iVar1 != 0) {
      FUN_1008_5d56(iVar1,iVar2);
      pcVar5 = (char *)0x1000;
      FUN_1000_1634(iVar1,iVar2);
    }
  }
  pcVar6 = pcVar5;
  if (*(int *)(iVar3 + 4) != 0) {
    pcVar6 = (char *)0x11d0;
    DELETEOBJECT(pcVar5,*(undefined2 *)(iVar3 + 4));
  }
  if (*(int *)(iVar3 + 0xd6) != 0) {
    DELETEOBJECT(pcVar6,*(undefined2 *)(iVar3 + 0xd6));
  }
  if (*(int *)(iVar3 + 0xd0) != 0 || *(int *)(iVar3 + 0xce) != 0) {
    FUN_1000_1634(*(undefined2 *)(iVar3 + 0xce),*(undefined2 *)(iVar3 + 0xd0));
  }
  FUN_1008_316c(iVar3,uVar4);
  return;
}



/* ============================================================
   1008:045e  FUN_1008_045e
   ============================================================ */

void __stdcall16far FUN_1008_045e(undefined4 param_1,int param_2,int param_3)

{
  undefined2 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  char *unaff_CS;
  
  if (param_3 != 0 || param_2 != 0) {
    uVar5 = (undefined2)((ulong)param_1 >> 0x10);
    iVar2 = (int)param_1;
    if ((*(int *)(iVar2 + 0xd8) == param_2) && (*(int *)(iVar2 + 0xda) == param_3)) {
      DAT_1080_0014 = 0;
    }
    iVar4 = 0;
    do {
      iVar3 = iVar4 * 4 + param_2;
      if (*(int *)(iVar3 + 0x16) != 0 || *(int *)(iVar3 + 0x14) != 0) {
        iVar3 = iVar4 * 4 + param_2;
        puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(iVar3 + 0x14) + 8);
        iVar3 = (*(code *)*puVar1)(unaff_CS,*(undefined2 *)(iVar3 + 0x14),
                                   *(undefined2 *)(iVar3 + 0x16));
        if ((iVar3 == 4) && (DAT_1080_0014 == 0)) {
          DAT_1080_0014 = 1;
        }
        else {
          iVar3 = iVar4 * 4 + param_2;
          unaff_CS = (char *)s_Lucky_Chip_1080_1004 + 4;
          FUN_1008_045e(iVar2,uVar5,*(undefined2 *)(iVar3 + 0x14),*(undefined2 *)(iVar3 + 0x16));
        }
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 4);
    if (param_3 != 0 || param_2 != 0) {
      FUN_1008_772c(param_2,param_3);
      FUN_1000_1634(param_2,param_3);
    }
  }
  return;
}



/* ============================================================
   1008:051a  FUN_1008_051a
   ============================================================ */

undefined2 __stdcall16far FUN_1008_051a(undefined2 *param_1,undefined2 param_2)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  
  uVar1 = FUN_1000_1a02(0,0);
  FUN_1000_1f56(uVar1);
  uVar1 = *param_1;
  uVar4 = LOADBITMAP(0x1000,0x3302,(char *)s_Lucky_Chip_1080_1004 + 4);
  iVar3 = (int)((ulong)uVar4 >> 0x10);
  param_1[2] = (int)uVar4;
  if ((int)uVar4 == 0) {
    FUN_1008_2d02(param_1,param_2,0,0x3310,(char *)s_Lucky_Chip_1080_1004 + 4);
    uVar1 = 0;
  }
  else {
    DAT_1080_298a = param_1[2];
    iVar2 = FUN_1000_1646(0xe,uVar1);
    if (iVar3 == 0 && iVar2 == 0) {
      param_1[5] = 0;
      param_1[4] = 0;
      iVar3 = 0;
    }
    else {
      uVar4 = FUN_1008_6c6a(iVar2,iVar3,param_1,param_2);
      iVar3 = (int)((ulong)uVar4 >> 0x10);
      param_1[4] = (int)uVar4;
      param_1[5] = iVar3;
    }
    iVar2 = FUN_1000_1646(0x6c,uVar1);
    if (iVar3 == 0 && iVar2 == 0) {
      param_1[9] = 0;
      param_1[8] = 0;
      iVar3 = 0;
    }
    else {
      uVar4 = FUN_1008_462a(iVar2,iVar3,param_1[4],param_1[5],param_1,param_2);
      iVar3 = (int)((ulong)uVar4 >> 0x10);
      param_1[8] = (int)uVar4;
      param_1[9] = iVar3;
    }
    iVar2 = FUN_1000_1646(0x12);
    if (iVar3 == 0 && iVar2 == 0) {
      param_1[7] = 0;
      param_1[6] = 0;
    }
    else {
      uVar4 = FUN_1008_5ca4(iVar2,iVar3,param_1[4],param_1[5],param_1,param_2);
      param_1[6] = (int)uVar4;
      param_1[7] = (int)((ulong)uVar4 >> 0x10);
    }
    uVar1 = 1;
  }
  return uVar1;
}



/* ============================================================
   1008:0630  FUN_1008_0630
   ============================================================ */

undefined2 __stdcall16far FUN_1008_0630(undefined2 *param_1)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  undefined2 unaff_SS;
  undefined2 *puVar4;
  undefined4 uVar5;
  undefined1 local_25e [296];
  undefined1 local_136 [296];
  undefined1 local_e [4];
  undefined2 local_a;
  undefined2 local_8;
  undefined4 local_6;
  
  uVar1 = FUN_1008_3fd6(*(undefined2 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 4),
                        *param_1);
  ((undefined2 *)param_1)[0x72] = uVar1;
  if (((undefined2 *)param_1)[0x6d] != 0 || ((undefined2 *)param_1)[0x6c] != 0) {
    FUN_1008_045e((undefined2 *)param_1,param_1._2_2_,((undefined2 *)param_1)[0x6c],
                  ((undefined2 *)param_1)[0x6d]);
  }
  FUN_1008_54da(((undefined2 *)param_1)[8],((undefined2 *)param_1)[9]);
  FUN_1008_67e6(((undefined2 *)param_1)[6],((undefined2 *)param_1)[7],0x332c,
                (char *)s_Lucky_Chip_1080_1004 + 4);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    ((undefined2 *)param_1)[iVar2 + 0x116] = 0;
    iVar2 = iVar3;
  } while (iVar3 < 2);
  ((undefined2 *)param_1)[0x7a] = 1;
  ((undefined2 *)param_1)[0x76] = 0;
  ((undefined2 *)param_1)[0x75] = 0;
  ((undefined2 *)param_1)[0x78] = 3;
  ((undefined2 *)param_1)[0x79] = 3;
  iVar3 = 0;
  ((undefined2 *)param_1)[0x7c] = 0;
  ((undefined2 *)param_1)[0x7b] = 0;
  ((undefined2 *)param_1)[0x7f] = 0;
  ((undefined2 *)param_1)[0x80] = 0;
  ((undefined2 *)param_1)[0x7d] = 0;
  ((undefined2 *)param_1)[0x7e] = 0;
  ((undefined2 *)param_1)[0x73] = 0;
  ((undefined2 *)param_1)[0x74] = 0;
  ((undefined2 *)param_1)[0x83] = 0;
  ((undefined2 *)param_1)[0x85] = 4;
  iVar2 = FUN_1008_3c88(5);
  ((undefined2 *)param_1)[0x86] = iVar2 + 3;
  ((undefined2 *)param_1)[0x81] = 2;
  iVar2 = FUN_1008_3c88(2);
  if (iVar2 == 1) {
    ((undefined2 *)param_1)[0x87] = 2;
    iVar2 = FUN_1008_3c88(2);
  }
  else {
    ((undefined2 *)param_1)[0x87] = 3;
    iVar2 = FUN_1008_3c88(5);
    iVar2 = iVar2 + 3;
  }
  ((undefined2 *)param_1)[0x88] = iVar2;
  ((undefined2 *)param_1)[0x81] = 3;
  if (((undefined2 *)param_1)[0x71] == 1) {
    ((undefined2 *)param_1)[0x87] = 1;
    ((undefined2 *)param_1)[0x89] = 0x13;
    ((undefined2 *)param_1)[0x8a] = 100;
    ((undefined2 *)param_1)[0x8b] = 7;
    ((undefined2 *)param_1)[0x8c] = 100;
    ((undefined2 *)param_1)[0x8d] = 0xd;
    ((undefined2 *)param_1)[0x8e] = 100;
    ((undefined2 *)param_1)[0x8f] = 0xb;
    ((undefined2 *)param_1)[0x90] = 100;
    ((undefined2 *)param_1)[0x91] = 10;
    ((undefined2 *)param_1)[0x92] = 100;
    ((undefined2 *)param_1)[0x93] = 0x14;
    ((undefined2 *)param_1)[0x94] = 100;
    ((undefined2 *)param_1)[0x95] = 5;
    ((undefined2 *)param_1)[0x96] = 100;
    ((undefined2 *)param_1)[0x97] = 3;
    ((undefined2 *)param_1)[0x98] = 100;
    ((undefined2 *)param_1)[0x99] = 8;
    ((undefined2 *)param_1)[0x9a] = 100;
    ((undefined2 *)param_1)[0x9b] = 9;
    ((undefined2 *)param_1)[0x9c] = 100;
    ((undefined2 *)param_1)[0x9d] = 0x10;
    ((undefined2 *)param_1)[0x9e] = 100;
    ((undefined2 *)param_1)[0x9f] = 0x11;
    ((undefined2 *)param_1)[0xa0] = 100;
    ((undefined2 *)param_1)[0xa1] = 0x12;
    ((undefined2 *)param_1)[0xa2] = 100;
    ((undefined2 *)param_1)[0xa3] = 0xc;
    ((undefined2 *)param_1)[0xa4] = 100;
    ((undefined2 *)param_1)[0xa5] = 6;
    ((undefined2 *)param_1)[0xa6] = 100;
    ((undefined2 *)param_1)[0xa7] = 2;
    ((undefined2 *)param_1)[0xa8] = 100;
    ((undefined2 *)param_1)[0xa9] = 0x15;
    ((undefined2 *)param_1)[0xaa] = 100;
    ((undefined2 *)param_1)[0xab] = 0xf;
    ((undefined2 *)param_1)[0xac] = 100;
    ((undefined2 *)param_1)[0xad] = 0xe;
    ((undefined2 *)param_1)[0xae] = 100;
    ((undefined2 *)param_1)[0x81] = 0x16;
  }
  *(undefined2 *)((int)*(undefined4 *)((undefined2 *)param_1 + 8) + 0x68) = 0;
  ((undefined2 *)param_1)[0x82] = 0;
  if (((undefined2 *)param_1)[0x71] == 0) {
    iVar2 = FUN_1000_1646(0x54);
    local_6._2_2_ = iVar3;
    if (iVar3 == 0 && iVar2 == 0) goto LAB_1008_08ba;
    uVar5 = FUN_1008_d4e0(iVar2,iVar3,0,1,(undefined2 *)param_1,param_1._2_2_);
  }
  else {
    iVar2 = FUN_1000_1646(0xe8);
    local_6._2_2_ = iVar3;
    if (iVar3 == 0 && iVar2 == 0) {
LAB_1008_08ba:
      ((undefined2 *)param_1)[0x6f] = 0;
      ((undefined2 *)param_1)[0x6e] = 0;
      goto LAB_1008_08c9;
    }
    uVar5 = FUN_1010_0ea0(iVar2,iVar3,0,1,(undefined2 *)param_1,param_1._2_2_);
  }
  ((undefined2 *)param_1)[0x6e] = (int)uVar5;
  ((undefined2 *)param_1)[0x6f] = (int)((ulong)uVar5 >> 0x10);
LAB_1008_08c9:
  puVar4 = (undefined2 *)
           FUN_1008_b946(((undefined2 *)param_1)[0x6e],((undefined2 *)param_1)[0x6f],0,local_e,
                         unaff_SS);
  local_a = *puVar4;
  local_8 = ((undefined2 *)puVar4)[1];
  ((undefined2 *)param_1)[0x75] = local_a;
  ((undefined2 *)param_1)[0x76] = local_8;
  ((undefined2 *)param_1)[0x77] = 0;
  uVar5 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 0x6e) + 8);
  uVar5 = *(undefined4 *)((int)uVar5 + ((undefined2 *)param_1)[0x75] * 4);
  *(undefined2 *)((int)uVar5 + ((undefined2 *)param_1)[0x76] * 2) = 0;
  uVar1 = ((undefined2 *)param_1)[0x6f];
  ((undefined2 *)param_1)[0x6c] = ((undefined2 *)param_1)[0x6e];
  ((undefined2 *)param_1)[0x6d] = uVar1;
  if (*(int *)*(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 6) + 10) != 0) {
    FUN_1010_edea(local_136,unaff_SS,(undefined2 *)param_1,param_1._2_2_);
    local_6 = FUN_1018_01fe(local_136,unaff_SS);
    if (((local_6 == 3) || (local_6 == 0xc)) || (local_6 == 0x32)) {
      uVar1 = (undefined2)((ulong)*(undefined4 *)((undefined2 *)param_1 + 6) >> 0x10);
      iVar2 = (int)*(undefined4 *)((undefined2 *)param_1 + 6);
      FUN_1010_81d4(*(undefined2 *)(iVar2 + 10),*(undefined2 *)(iVar2 + 0xc),1);
    }
    else {
      SHOWWINDOW(0x1018,0);
    }
    FUN_1010_ef2e(local_136,unaff_SS);
  }
  FUN_1010_edea(local_25e,unaff_SS,(undefined2 *)param_1,param_1._2_2_);
  if (((undefined2 *)param_1)[0x68] != 0 || ((undefined2 *)param_1)[0x67] != 0) {
    FUN_1018_0342(local_25e,unaff_SS,((undefined2 *)param_1)[0x67],((undefined2 *)param_1)[0x68]);
  }
  FUN_1010_ef2e(local_25e,unaff_SS);
  return 1;
}



/* ============================================================
   1008:0a3c  FUN_1008_0a3c
   ============================================================ */

undefined2 __stdcall16far FUN_1008_0a3c(char *param_1)

{
  undefined2 *puVar1;
  char *pcVar2;
  int iVar3;
  char *pcVar4;
  undefined2 *puVar5;
  char *pcVar6;
  undefined2 uVar7;
  undefined2 unaff_SS;
  char *pcVar8;
  char *pcVar9;
  undefined1 local_23a [296];
  char local_112 [136];
  undefined1 local_8a;
  undefined2 local_89 [63];
  undefined2 local_a;
  undefined2 local_8;
  char local_6;
  char local_5;
  char local_4;
  
  pcVar6 = (char *)((ulong)param_1 >> 0x10);
  pcVar4 = (char *)param_1;
  FUN_1008_54da(*(undefined2 *)(pcVar4 + 0x10),*(undefined2 *)(pcVar4 + 0x12));
  local_8a = DAT_1008_336c;
  puVar5 = local_89;
  for (iVar3 = 0x3f; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar1 = puVar5;
    puVar5 = puVar5 + 1;
    *puVar1 = 0;
  }
  *(undefined1 *)puVar5 = 0;
  pcVar8 = pcVar4;
  pcVar9 = pcVar6;
  iVar3 = FUN_1008_0c70(pcVar4,pcVar6,&local_8a,unaff_SS);
  if (iVar3 != -1) {
    if (iVar3 != 0) {
      pcVar8 = local_112;
      pcVar2 = (char *)OPENFILE((char *)s_Lucky_Chip_1080_1004 + 4,0,pcVar8,unaff_SS,&local_8a);
      if (pcVar2 == (char *)0xffff) {
        pcVar9 = (char *)0x33a2;
      }
      else {
        _LREAD(0x11d0,3,&local_6,unaff_SS);
        if (((local_6 == 'D') && (local_5 == 'A')) && (local_4 == 'H')) {
          local_a = *(undefined2 *)0x33ec;
          local_8 = *(undefined2 *)0x33ee;
          pcVar9 = pcVar2;
          _LREAD(0x11d0,3,&local_a,unaff_SS);
          pcVar8 = (char *)0x1000;
          iVar3 = FUN_1000_16e6(&local_a);
          if (iVar3 == 0) {
            if (*(int *)(pcVar4 + 0xda) != 0 || *(int *)(pcVar4 + 0xd8) != 0) {
              pcVar8 = (char *)s_Lucky_Chip_1080_1004 + 4;
              FUN_1008_045e(pcVar4,pcVar6,*(undefined2 *)(pcVar4 + 0xd8),
                            *(undefined2 *)(pcVar4 + 0xda));
            }
            pcVar9 = pcVar2;
            _LREAD(pcVar8,0x14c,pcVar4 + 0xe2,pcVar6);
            pcVar8 = pcVar2;
            _LREAD(0x11d0,2,*(int *)(pcVar4 + 0x10) + 0x68,*(undefined2 *)(pcVar4 + 0x12));
            FUN_1008_0e1c(pcVar4,pcVar6,0,1,0,0,pcVar2);
            iVar3 = _LCLOSE((char *)s_Lucky_Chip_1080_1004 + 4,pcVar2,pcVar8,pcVar9);
            if (iVar3 == -1) {
              FUN_1008_2d02(pcVar4,pcVar6,0,0x3434,(char *)s_Lucky_Chip_1080_1004 + 4);
              if (*(int *)(pcVar4 + 0xda) != 0 || *(int *)(pcVar4 + 0xd8) != 0) {
                FUN_1008_045e(pcVar4,pcVar6,*(undefined2 *)(pcVar4 + 0xd8),
                              *(undefined2 *)(pcVar4 + 0xda));
              }
              DAT_1080_1be6 = 0;
              FUN_1008_169c(pcVar4,pcVar6);
              return 0;
            }
            if (*(int *)*(undefined4 *)((int)*(undefined4 *)(pcVar4 + 0xc) + 10) != 0) {
              uVar7 = (undefined2)((ulong)*(undefined4 *)(pcVar4 + 0xc) >> 0x10);
              iVar3 = (int)*(undefined4 *)(pcVar4 + 0xc);
              FUN_1010_81d4(*(undefined2 *)(iVar3 + 10),*(undefined2 *)(iVar3 + 0xc),1);
            }
            FUN_1010_edea(local_23a,unaff_SS,pcVar4,pcVar6);
            if (*(int *)(pcVar4 + 0xd0) != 0 || *(int *)(pcVar4 + 0xce) != 0) {
              FUN_1018_0342(local_23a,unaff_SS,*(undefined2 *)(pcVar4 + 0xce),
                            *(undefined2 *)(pcVar4 + 0xd0));
            }
            FUN_1010_ef2e(local_23a,unaff_SS);
            return 1;
          }
          pcVar8 = (char *)s_Lucky_Chip_1080_1004 + 4;
        }
        else {
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          pcVar8 = (char *)0x33be;
        }
      }
    }
    FUN_1008_2d02(pcVar4,pcVar6,0,pcVar8,pcVar9);
  }
  REDRAWWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,0);
  return 0;
}



/* ============================================================
   1008:0c70  FUN_1008_0c70
   ============================================================ */

undefined2 __stdcall16far FUN_1008_0c70(undefined4 param_1)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  undefined4 uVar3;
  undefined2 uVar4;
  int iVar5;
  undefined2 *puVar6;
  undefined2 *puVar7;
  undefined2 unaff_SS;
  long lVar8;
  undefined2 local_68;
  undefined2 local_66;
  undefined2 local_64;
  undefined2 *local_60;
  undefined2 local_20 [5];
  undefined1 local_16;
  undefined1 local_f [13];
  
  puVar7 = local_20;
  puVar6 = (undefined2 *)0x3448;
  for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
    puVar2 = puVar7;
    puVar7 = puVar7 + 1;
    puVar1 = puVar6;
    puVar6 = puVar6 + 1;
    *puVar2 = *puVar1;
  }
  *(undefined1 *)puVar7 = *(undefined1 *)puVar6;
  puVar7 = (undefined2 *)local_f;
  for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
    puVar1 = puVar7;
    puVar7 = puVar7 + 1;
    *puVar1 = 0;
  }
  *(undefined1 *)puVar7 = 0;
  local_16 = 0;
  local_f[0] = 0;
  FUN_1000_1e96(&local_68);
  local_68 = 0x48;
  local_66 = 0;
  uVar3 = *(undefined4 *)((int)param_1 + 0xc);
  local_64 = *(undefined2 *)((int)uVar3 + 8);
  local_60 = local_20;
  iVar5 = GETOPENFILENAME(0x1000,&local_68);
  if (iVar5 == 0) {
    lVar8 = COMMDLGEXTENDEDERROR(0x11d0);
    if (lVar8 == 0) {
      uVar4 = 0xffff;
    }
    else {
      uVar4 = 0;
    }
  }
  else {
    uVar4 = 1;
  }
  return uVar4;
}



/* ============================================================
   1008:0d46  FUN_1008_0d46
   ============================================================ */

undefined2 __stdcall16far FUN_1008_0d46(undefined4 param_1)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  undefined4 uVar3;
  undefined2 uVar4;
  int iVar5;
  undefined2 *puVar6;
  undefined2 *puVar7;
  undefined2 unaff_SS;
  long lVar8;
  undefined2 local_68;
  undefined2 local_66;
  undefined2 local_64;
  undefined2 *local_60;
  undefined2 local_20 [5];
  undefined1 local_16;
  undefined1 local_f [13];
  
  puVar7 = local_20;
  puVar6 = (undefined2 *)0x3448;
  for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
    puVar2 = puVar7;
    puVar7 = puVar7 + 1;
    puVar1 = puVar6;
    puVar6 = puVar6 + 1;
    *puVar2 = *puVar1;
  }
  *(undefined1 *)puVar7 = *(undefined1 *)puVar6;
  puVar7 = (undefined2 *)local_f;
  for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
    puVar1 = puVar7;
    puVar7 = puVar7 + 1;
    *puVar1 = 0;
  }
  *(undefined1 *)puVar7 = 0;
  local_16 = 0;
  local_f[0] = 0;
  FUN_1000_1e96(&local_68);
  local_68 = 0x48;
  local_66 = 0;
  uVar3 = *(undefined4 *)((int)param_1 + 0xc);
  local_64 = *(undefined2 *)((int)uVar3 + 8);
  local_60 = local_20;
  iVar5 = GETSAVEFILENAME(0x1000,&local_68);
  if (iVar5 == 0) {
    lVar8 = COMMDLGEXTENDEDERROR(0x11d0);
    if (lVar8 == 0) {
      uVar4 = 0xffff;
    }
    else {
      uVar4 = 0;
    }
  }
  else {
    uVar4 = 1;
  }
  return uVar4;
}



/* ============================================================
   1008:0e1c  FUN_1008_0e1c
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_0e1c(undefined4 param_1,int param_2,int param_3,int param_4,int param_5,undefined2 param_6)

{
  undefined2 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined2 uVar5;
  int extraout_DX;
  undefined2 extraout_DX_00;
  int extraout_DX_01;
  int iVar6;
  undefined2 *puVar7;
  undefined2 uVar8;
  undefined2 unaff_CS;
  char *pcVar9;
  char *pcVar10;
  undefined2 unaff_SS;
  undefined4 *puVar11;
  undefined4 uVar12;
  undefined4 *puVar13;
  undefined2 uVar14;
  undefined4 local_28;
  undefined2 local_24;
  undefined2 local_22;
  undefined2 local_20;
  int local_1a [4];
  int local_12;
  int local_10;
  undefined2 local_e;
  undefined4 local_c;
  int local_6;
  int local_4;
  
  uVar8 = param_6;
  _LREAD(unaff_CS,2,&local_e,unaff_SS);
  switch(local_e) {
  case 0:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x54,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      local_c = (undefined4 *)FUN_1008_d4e0(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 1:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x54,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)FUN_1010_4044(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 2:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x54,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)FUN_1010_5fc4(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 3:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(100,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)FUN_1010_6a56(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 4:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x66,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)FUN_1010_55ac(iVar2,iVar4,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 5:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0xe8,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)FUN_1010_0ea0(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 6:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x6c,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      local_c = (undefined4 *)FUN_1008_e07a(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  default:
    FUN_1008_2d02((int)param_1,param_1._2_2_,0,0x3482,(char *)s_Lucky_Chip_1080_1004 + 4);
    return 0;
  case 9:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x5c,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)FUN_1010_3a96(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 10:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x5a,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)FUN_1010_33b4(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 0xb:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x54,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)FUN_1010_c80e(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 0xc:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x56,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)
                FUN_1010_86a0(iVar2,iVar4,1,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 0xd:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x58,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)FUN_1010_4f76(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 0xe:
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x56,uVar8);
    local_4 = iVar4;
    if (iVar4 != 0 || iVar2 != 0) {
      pcVar9 = (char *)0x1010;
      local_c = (undefined4 *)
                FUN_1010_cb9c(iVar2,iVar4,0,param_2,param_3,(int)param_1,param_1._2_2_);
      puVar3 = (undefined4 *)local_c;
      goto LAB_1008_10cd;
    }
    break;
  case 0x11:
    pcVar9 = (char *)0x1000;
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1646(0x54,uVar8);
    local_4 = iVar4;
    if (iVar4 == 0 && iVar2 == 0) {
      puVar3 = (undefined4 *)0x0;
      local_c = (undefined4 *)ZEXT24((undefined4 *)local_c);
    }
    else {
      pcVar9 = (char *)0x1010;
      puVar11 = (undefined4 *)FUN_1010_d33a(iVar2,iVar4,param_2,param_3,(int)param_1,param_1._2_2_);
      local_c = (undefined4 *)CONCAT22((int)((ulong)puVar11 >> 0x10),(undefined4 *)local_c);
      puVar3 = (undefined4 *)puVar11;
    }
    goto LAB_1008_10cd;
  }
  pcVar9 = (char *)0x1000;
  puVar3 = (undefined4 *)0x0;
  local_c = (undefined4 *)0x0;
LAB_1008_10cd:
  uVar8 = (undefined2)((ulong)local_c >> 0x10);
  if (param_5 == 0 && param_4 == 0) {
    *(undefined2 *)((int)param_1 + 0xd8) = puVar3;
    *(undefined2 *)((int)param_1 + 0xda) = uVar8;
    *(undefined2 *)(puVar3 + 4) = 0;
    *(undefined2 *)((int)puVar3 + 0xe) = 0;
  }
  else {
    puVar1 = (undefined2 *)((int)*puVar3 + 8);
    iVar4 = (*(code *)*puVar1)(pcVar9,puVar3,uVar8);
    if (iVar4 == 4) {
      *(int *)((int)puVar3 + 0x62) = param_4;
      *(int *)(puVar3 + 0x19) = param_5;
      *(undefined2 *)(param_4 + 0x14) = puVar3;
      *(undefined2 *)(param_4 + 0x16) = local_c._2_2_;
      uVar12 = *(undefined4 *)((int)*(undefined4 *)(param_4 + 0xe) + 0x14);
      uVar8 = (undefined2)((ulong)uVar12 >> 0x10);
      iVar4 = (int)uVar12;
      *(int *)((int)puVar3 + 0xe) = iVar4;
      *(undefined2 *)(puVar3 + 4) = uVar8;
      *(undefined2 *)(iVar4 + 0x14) = puVar3;
      *(undefined2 *)(iVar4 + 0x16) = local_c._2_2_;
    }
    else {
      *(int *)((int)puVar3 + 0xe) = param_4;
      *(int *)(puVar3 + 4) = param_5;
      *(undefined2 *)(param_4 + param_2 * 4 + 0x14) = puVar3;
      *(undefined2 *)(param_4 + param_2 * 4 + 0x16) = local_c._2_2_;
    }
  }
  uVar5 = param_6;
  _LREAD(pcVar9,8,local_1a,unaff_SS);
  uVar8 = param_6;
  _LREAD(0x11d0,2,&local_10,unaff_SS);
  if (local_10 == 1) {
    *(undefined2 *)((int)param_1 + 0xdc) = puVar3;
    *(undefined2 *)((int)param_1 + 0xde) = local_c._2_2_;
  }
  puVar1 = (undefined2 *)((int)*puVar3 + 0xc);
  puVar13 = puVar3;
  uVar14 = local_c._2_2_;
  iVar4 = (*(code *)*puVar1)(0x11d0,puVar3,local_c._2_2_,(int)puVar3 + 0x3a,local_c._2_2_,param_6,
                             uVar8,uVar5);
  _LREAD(0x11d0,iVar4 + 0x1a,puVar13,uVar14);
  local_c._0_2_ = puVar3;
  for (iVar4 = 0; iVar4 < 0x28; iVar4 = iVar4 + 1) {
    uVar8 = (undefined2)((ulong)puVar3[2] >> 0x10);
    puVar7 = (undefined2 *)((int)puVar3[2] + iVar4 * 4);
    _LREAD(0x11d0,0x48,*puVar7,puVar7[1]);
  }
  uVar14 = param_6;
  _LREAD(0x11d0,0x5dc,*(undefined2 *)((int)puVar3 + 0x32),*(undefined2 *)(puVar3 + 0xd));
  local_28 = (int *)CONCAT22(local_c._2_2_,puVar3 + 0xc);
  uVar5 = param_6;
  _LREAD(0x11d0,2,puVar3 + 0xc,local_c._2_2_);
  uVar8 = extraout_DX_00;
  FUN_1000_1634(*(undefined2 *)(puVar3 + 0xb),*(undefined2 *)((int)puVar3 + 0x2e),uVar5,uVar14);
  uVar5 = FUN_1000_1646(*local_28 * 0xc);
  *(undefined2 *)(puVar3 + 0xb) = uVar5;
  *(undefined2 *)((int)puVar3 + 0x2e) = uVar8;
  uVar5 = param_6;
  _LREAD(0x1000,*local_28 * 0xc,*(undefined2 *)(puVar3 + 0xb),uVar8);
  iVar4 = *(int *)((int)puVar3 + 0x36);
  iVar2 = *(int *)(puVar3 + 0xe);
  iVar6 = 0;
  local_4 = iVar2;
  if (iVar2 != 0 || iVar4 != 0) {
    FUN_1008_af18(iVar4,iVar2);
    iVar6 = extraout_DX_01;
    FUN_1000_1634(iVar4,iVar2,uVar5);
  }
  pcVar9 = (char *)0x1000;
  iVar4 = FUN_1000_1646(6);
  local_6 = iVar6;
  if (iVar6 == 0 && iVar4 == 0) {
    uVar12 = 0;
  }
  else {
    pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
    uVar12 = FUN_1008_aefa(iVar4,iVar6);
  }
  *(undefined2 *)((int)puVar3 + 0x36) = (int)uVar12;
  *(int *)(puVar3 + 0xe) = (int)((ulong)uVar12 >> 0x10);
  pcVar10 = (char *)0x11d0;
  uVar8 = param_6;
  _LREAD(pcVar9,2,&local_12,unaff_SS);
  local_4 = 0;
  for (iVar4 = 0; iVar4 < local_12; iVar4 = iVar4 + 1) {
    _LREAD(pcVar10,10,&local_24,unaff_SS);
    pcVar10 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_af5a(*(undefined2 *)((int)puVar3 + 0x36),*(undefined2 *)(puVar3 + 0xe),local_20,
                  local_22,local_24);
  }
  iVar4 = 0;
  do {
    if ((local_1a[iVar4] == 1) &&
       ((puVar1 = (undefined2 *)((int)*puVar3 + 8),
        iVar2 = (*(code *)*puVar1)(pcVar10,puVar3,local_c._2_2_,uVar8), iVar2 != 2 || (iVar4 != 0)))
       ) {
      pcVar10 = (char *)s_Lucky_Chip_1080_1004 + 4;
      FUN_1008_0e1c((int)param_1,param_1._2_2_,iVar4,param_3 + 1,puVar3,local_c._2_2_,param_6);
    }
    iVar4 = iVar4 + 1;
  } while (iVar4 < 4);
  return 1;
}



/* ============================================================
   1008:134e  FUN_1008_134e
   ============================================================ */

undefined2 __stdcall16far FUN_1008_134e(undefined4 param_1)

{
  undefined2 *puVar1;
  int iVar2;
  int iVar3;
  undefined2 *puVar4;
  undefined2 unaff_SS;
  undefined4 uVar5;
  undefined2 in_stack_0000fee0;
  char *in_stack_0000fee2;
  int iVar6;
  int iVar7;
  undefined1 local_10c [136];
  undefined1 local_84;
  undefined2 local_83 [63];
  int local_4;
  
  if (*(int *)((int)param_1 + 0xe2) == 1) {
    FUN_1008_67e6(*(undefined2 *)((int)param_1 + 0xc),*(undefined2 *)((int)param_1 + 0xe),0x34a2,
                  (char *)s_Lucky_Chip_1080_1004 + 4);
  }
  else {
    local_84 = DAT_1008_336c;
    puVar4 = local_83;
    for (iVar2 = 0x3f; iVar2 != 0; iVar2 = iVar2 + -1) {
      puVar1 = puVar4;
      puVar4 = puVar4 + 1;
      *puVar1 = 0;
    }
    *(undefined1 *)puVar4 = 0;
    uVar5 = FUN_1008_0d46((int)param_1,param_1._2_2_,&local_84,unaff_SS);
    iVar3 = (int)((ulong)uVar5 >> 0x10);
    local_4 = (int)uVar5;
    iVar2 = FUN_1000_1dd2(&local_84);
    if (iVar3 == 0 && iVar2 == 0) {
      FUN_1000_1656(&local_84);
    }
    if (local_4 != -1) {
      if (local_4 != 0) {
        in_stack_0000fee2 = (char *)0x1408;
        iVar2 = OPENFILE(0x1000,0x1001,local_10c,unaff_SS,&local_84);
        if (iVar2 != -1) {
          _LWRITE(0x11d0,3,0x3520,(char *)s_Lucky_Chip_1080_1004 + 4);
          iVar7 = iVar2;
          _LWRITE(0x11d0,3,0x10,0x1080);
          iVar6 = iVar2;
          _LWRITE(0x11d0,0x14c,(int)param_1 + 0xe2,param_1._2_2_);
          iVar3 = iVar2;
          _LWRITE(0x11d0,2,*(int *)((int)param_1 + 0x10) + 0x68,*(undefined2 *)((int)param_1 + 0x12)
                 );
          FUN_1008_14a4((int)param_1,param_1._2_2_,*(undefined2 *)((int)param_1 + 0xd8),
                        *(undefined2 *)((int)param_1 + 0xda),iVar2);
          iVar2 = _LCLOSE((char *)s_Lucky_Chip_1080_1004 + 4,iVar2,iVar3,iVar6,iVar7);
          if (iVar2 != -1) {
            FUN_1008_67e6(*(undefined2 *)((int)param_1 + 0xc),*(undefined2 *)((int)param_1 + 0xe),
                          0x3524,(char *)s_Lucky_Chip_1080_1004 + 4);
            return 1;
          }
          in_stack_0000fee2 = (char *)s_Lucky_Chip_1080_1004 + 4;
          in_stack_0000fee0 = 0x3434;
        }
      }
      FUN_1008_2d02((int)param_1,param_1._2_2_,0,in_stack_0000fee0,in_stack_0000fee2);
    }
  }
  return 0;
}



/* ============================================================
   1008:14a4  FUN_1008_14a4
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_14a4(undefined4 param_1,undefined4 *param_2,int param_3,undefined2 param_4)

{
  int *piVar1;
  undefined2 *puVar2;
  int iVar3;
  int iVar4;
  undefined2 *puVar5;
  int iVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  undefined2 unaff_CS;
  char *pcVar10;
  undefined2 unaff_SS;
  undefined2 uVar11;
  undefined4 *puVar12;
  undefined4 local_16;
  uint local_12 [4];
  undefined2 local_a;
  undefined2 local_8;
  int local_6;
  int local_4;
  
  puVar2 = (undefined2 *)((int)*param_2 + 8);
  local_a = (*(code *)*puVar2)();
  uVar8 = param_4;
  _LWRITE(unaff_CS,2,&local_a,unaff_SS);
  local_4 = 0;
  for (iVar4 = 0; iVar4 < 4; iVar4 = iVar4 + 1) {
    local_12[iVar4] =
         (uint)(*(int *)((int)param_2 + iVar4 * 4 + 0x16) != 0 || *(int *)(param_2 + iVar4 + 5) != 0
               );
  }
  uVar11 = param_4;
  _LWRITE(0x11d0,8,local_12,unaff_SS);
  local_8 = 0;
  uVar7 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  if (((undefined4 *)*(int *)(iVar4 + 0xdc) == param_2) && (*(int *)(iVar4 + 0xde) == param_3)) {
    local_8 = 1;
  }
  uVar9 = param_4;
  _LWRITE(0x11d0,2,&local_8,unaff_SS);
  puVar2 = (undefined2 *)((int)*param_2 + 0xc);
  puVar12 = param_2;
  iVar6 = param_3;
  iVar3 = (*(code *)*puVar2)(0x11d0,param_2,param_3,(int)param_2 + 0x3a,param_3,param_4,uVar9,uVar11
                             ,uVar8);
  _LWRITE(0x11d0,iVar3 + 0x1a,puVar12,iVar6);
  local_4 = 0;
  for (iVar6 = 0; iVar6 < 0x28; iVar6 = iVar6 + 1) {
    uVar8 = (undefined2)((ulong)param_2[2] >> 0x10);
    puVar5 = (undefined2 *)((int)param_2[2] + iVar6 * 4);
    _LWRITE(0x11d0,0x48,*puVar5,puVar5[1]);
  }
  _LWRITE(0x11d0,0x5dc,*(undefined2 *)((int)param_2 + 0x32),*(undefined2 *)(param_2 + 0xd));
  local_16 = (int *)CONCAT22(param_3,param_2 + 0xc);
  _LWRITE(0x11d0,2,param_2 + 0xc,param_3);
  uVar11 = param_4;
  _LWRITE(0x11d0,*local_16 * 0xc,*(undefined2 *)(param_2 + 0xb),*(undefined2 *)((int)param_2 + 0x2e)
         );
  uVar8 = param_4;
  _LWRITE(0x11d0,2,*(undefined2 *)((int)param_2 + 0x36),*(undefined2 *)(param_2 + 0xe));
  uVar9 = (undefined2)((ulong)*(undefined4 *)((int)param_2 + 0x36) >> 0x10);
  iVar6 = (int)*(undefined4 *)((int)param_2 + 0x36);
  local_6 = *(int *)(iVar6 + 2);
  local_4 = *(int *)(iVar6 + 4);
  iVar6 = local_6;
  while (pcVar10 = (char *)0x11d0, local_4 != 0 || iVar6 != 0) {
    _LWRITE(0x11d0,10,iVar6,local_4);
    piVar1 = (int *)(iVar6 + 6);
    local_4 = *(int *)(iVar6 + 8);
    iVar6 = *piVar1;
  }
  for (iVar6 = 0; iVar6 < 4; iVar6 = iVar6 + 1) {
    if ((*(int *)((int)param_2 + iVar6 * 4 + 0x16) != 0 || *(int *)(param_2 + iVar6 + 5) != 0) &&
       ((puVar2 = (undefined2 *)((int)*(undefined4 *)param_2[iVar6 + 5] + 8),
        iVar3 = (*(code *)*puVar2)(pcVar10,*(undefined2 *)(param_2 + iVar6 + 5),
                                   *(undefined2 *)((int)param_2 + iVar6 * 4 + 0x16),uVar8,uVar11),
        iVar3 != 4 ||
        (puVar2 = (undefined2 *)((int)*param_2 + 8),
        iVar3 = (*(code *)*puVar2)(pcVar10,param_2,param_3), iVar3 != 2)))) {
      pcVar10 = (char *)s_Lucky_Chip_1080_1004 + 4;
      FUN_1008_14a4(iVar4,uVar7,*(undefined2 *)(param_2 + iVar6 + 5),
                    *(undefined2 *)((int)param_2 + iVar6 * 4 + 0x16),param_4);
    }
  }
  return 1;
}



/* ============================================================
   1008:169c  FUN_1008_169c
   ============================================================ */

undefined2 __stdcall16far FUN_1008_169c(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  int extraout_DX;
  char *pcVar3;
  undefined2 unaff_SS;
  undefined2 *puVar4;
  undefined4 uVar5;
  undefined1 local_e [4];
  undefined2 local_a;
  undefined2 local_8;
  int local_4;
  
  *(undefined2 *)(param_1 + 0xf4) = 1;
  *(undefined2 *)(param_1 + 0xec) = 0;
  *(undefined2 *)(param_1 + 0xea) = 0;
  *(undefined2 *)(param_1 + 0xf0) = 3;
  *(undefined2 *)(param_1 + 0xf2) = 3;
  iVar2 = 0;
  *(undefined2 *)(param_1 + 0xf8) = 0;
  *(undefined2 *)(param_1 + 0xf6) = 0;
  *(undefined2 *)(param_1 + 0xfe) = 0;
  *(undefined2 *)(param_1 + 0x100) = 0;
  *(undefined2 *)(param_1 + 0xfa) = 0;
  *(undefined2 *)(param_1 + 0x106) = 0;
  *(undefined2 *)(param_1 + 0x10a) = 4;
  iVar1 = FUN_1008_3c88(5);
  *(int *)(param_1 + 0x10c) = iVar1 + 0x12;
  *(undefined2 *)(param_1 + 0x10e) = 1;
  *(undefined2 *)(param_1 + 0x112) = 3;
  iVar1 = FUN_1008_3c88(5);
  *(int *)(param_1 + 0x114) = iVar1 + 0x12;
  *(undefined2 *)(param_1 + 0x116) = 2;
  *(undefined2 *)(param_1 + 0x118) = 3;
  *(undefined2 *)(param_1 + 0x11a) = 5;
  iVar1 = FUN_1008_3c88(5);
  *(int *)(param_1 + 0x11c) = iVar1 + 8;
  *(undefined2 *)(param_1 + 0x11e) = 6;
  iVar1 = FUN_1008_3c88(5);
  *(int *)(param_1 + 0x120) = iVar1 + 8;
  *(undefined2 *)(param_1 + 0x122) = 8;
  *(undefined2 *)(param_1 + 0x124) = 7;
  *(undefined2 *)(param_1 + 0x126) = 9;
  *(undefined2 *)(param_1 + 0x128) = 2;
  *(undefined2 *)(param_1 + 0x12a) = 0x13;
  *(undefined2 *)(param_1 + 300) = 5;
  *(undefined2 *)(param_1 + 0x12e) = 0xe;
  *(undefined2 *)(param_1 + 0x130) = 1;
  *(undefined2 *)(param_1 + 0x132) = 0xb;
  iVar1 = FUN_1008_3c88(3);
  *(int *)(param_1 + 0x134) = iVar1 + 3;
  *(undefined2 *)(param_1 + 0x102) = 0xc;
  *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x10) + 0x68) = 0;
  *(undefined2 *)(param_1 + 0x104) = 0;
  pcVar3 = (char *)s_Lucky_Chip_1080_1004 + 4;
  iVar1 = FUN_1008_3c88(4);
  if (iVar1 == 1) {
    iVar1 = FUN_1000_1646(0xe8);
    local_4 = iVar2;
    if (iVar2 == 0 && iVar1 == 0) {
LAB_1008_1820:
      pcVar3 = (char *)0x1000;
      *(undefined2 *)(param_1 + 0xde) = 0;
      *(undefined2 *)(param_1 + 0xdc) = 0;
      goto LAB_1008_182f;
    }
    pcVar3 = (char *)0x1010;
    uVar5 = FUN_1010_0ea0(iVar1,iVar2,0,1,param_1,param_2);
  }
  else if (iVar1 == 2) {
    iVar1 = FUN_1000_1646(0x6c);
    local_4 = iVar2;
    if (iVar2 == 0 && iVar1 == 0) goto LAB_1008_1820;
    pcVar3 = (char *)s_Lucky_Chip_1080_1004 + 4;
    uVar5 = FUN_1008_e07a(iVar1,iVar2,0,1,param_1,param_2);
  }
  else if (iVar1 == 3) {
    iVar1 = FUN_1000_1646(0x5a);
    local_4 = iVar2;
    if (iVar2 == 0 && iVar1 == 0) goto LAB_1008_1820;
    pcVar3 = (char *)0x1010;
    uVar5 = FUN_1010_33b4(iVar1,iVar2,0,1,param_1,param_2);
  }
  else {
    if (iVar1 != 4) goto LAB_1008_182f;
    iVar1 = FUN_1000_1646(0x54);
    local_4 = iVar2;
    if (iVar2 == 0 && iVar1 == 0) goto LAB_1008_1820;
    pcVar3 = (char *)0x1010;
    uVar5 = FUN_1010_d8f4(iVar1,iVar2,0,1,param_1,param_2);
  }
  *(undefined2 *)(param_1 + 0xdc) = (int)uVar5;
  *(undefined2 *)(param_1 + 0xde) = (int)((ulong)uVar5 >> 0x10);
LAB_1008_182f:
  puVar4 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 0x28);
  (*(code *)*puVar4)(pcVar3,*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde));
  DAT_1080_1be6 = 0;
  puVar4 = (undefined2 *)
           FUN_1008_b946(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde),0,local_e,
                         unaff_SS);
  local_a = *puVar4;
  local_8 = ((undefined2 *)puVar4)[1];
  *(undefined2 *)(param_1 + 0xea) = local_a;
  *(undefined2 *)(param_1 + 0xec) = local_8;
  *(undefined2 *)(param_1 + 0xee) = 0;
  iVar1 = *(int *)(param_1 + 0xde);
  *(undefined2 *)(param_1 + 0xd8) = *(undefined2 *)(param_1 + 0xdc);
  *(int *)(param_1 + 0xda) = iVar1;
  if (*(int *)*(undefined4 *)((int)*(undefined4 *)(param_1 + 0xc) + 10) != 0) {
    SHOWWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,5);
    FUN_1008_198e(param_1,param_2);
    iVar1 = extraout_DX;
  }
  if (*(int *)(param_1 + 0xd0) != 0 || *(int *)(param_1 + 0xce) != 0) {
    FUN_1000_1634(*(undefined2 *)(param_1 + 0xce),*(undefined2 *)(param_1 + 0xd0));
  }
  iVar2 = FUN_1000_1646(0x26);
  if (iVar1 == 0 && iVar2 == 0) {
    uVar5 = 0;
  }
  else {
    local_4 = iVar1;
    uVar5 = FUN_1010_ed68(iVar2,iVar1);
  }
  *(undefined2 *)(param_1 + 0xce) = (int)uVar5;
  *(undefined2 *)(param_1 + 0xd0) = (int)((ulong)uVar5 >> 0x10);
  return 1;
}



/* ============================================================
   1008:198e  FUN_1008_198e
   ============================================================ */

void __stdcall16far FUN_1008_198e(int param_1,undefined2 param_2)

{
  undefined2 *puVar1;
  int iVar2;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar3;
  undefined2 uVar4;
  int local_a;
  int local_8;
  int local_6;
  int local_4;
  
  if (*(int *)*(undefined4 *)((int)*(undefined4 *)(param_1 + 0xc) + 10) == 0) {
    return;
  }
  uVar4 = *(undefined2 *)*(undefined4 *)((int)*(undefined4 *)(param_1 + 0xc) + 10);
  GETWINDOWRECT(unaff_CS,&local_a,unaff_SS);
  puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 8);
  iVar2 = (*(code *)*puVar1)(0x11d0,*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde),
                             uVar4);
  if (iVar2 == 5) {
LAB_1008_1a2f:
    uVar4 = 0xc;
  }
  else {
    puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 8);
    iVar2 = (*(code *)*puVar1)(0x11d0,*(undefined2 *)(param_1 + 0xdc),
                               *(undefined2 *)(param_1 + 0xde));
    if (iVar2 != 6) {
      puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 8);
      iVar2 = (*(code *)*puVar1)(0x11d0,*(undefined2 *)(param_1 + 0xdc),
                                 *(undefined2 *)(param_1 + 0xde));
      if (iVar2 != 10) {
        puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 8);
        iVar2 = (*(code *)*puVar1)(0x11d0,*(undefined2 *)(param_1 + 0xdc),
                                   *(undefined2 *)(param_1 + 0xde));
        if (iVar2 == 0x10) {
          uVar4 = 0x18;
        }
        else {
          uVar4 = 0x90;
        }
        uVar3 = 0x114;
        goto LAB_1008_1a87;
      }
      goto LAB_1008_1a2f;
    }
    uVar4 = 0xe4;
  }
  uVar3 = 0xc;
LAB_1008_1a87:
  MOVEWINDOW(0x11d0,1,local_4 - local_8,local_6 - local_a,uVar3,uVar4);
  REDRAWWINDOW(0x11d0,1,0,0,0);
  return;
}



/* ============================================================
   1008:1abc  FUN_1008_1abc
   ============================================================ */

undefined2 __stdcall16far FUN_1008_1abc(int param_1,undefined2 param_2)

{
  FUN_1008_6d30(*(undefined2 *)(param_1 + 8),*(undefined2 *)(param_1 + 10),
                *(undefined2 *)(param_1 + 0xc),*(undefined2 *)(param_1 + 0xe),
                *(undefined2 *)(param_1 + 0x10),*(undefined2 *)(param_1 + 0x12));
  return 1;
}



/* ============================================================
   1008:1aec  FUN_1008_1aec
   ============================================================ */

undefined2 __stdcall16far FUN_1008_1aec(void)

{
  int iVar1;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 local_10;
  
  while( true ) {
    iVar1 = GETMESSAGE(unaff_CS,0,0,0,&stack0xffec,unaff_SS);
    if (iVar1 == 0) break;
    TRANSLATEMESSAGE(0x11d0,&stack0xffec);
    unaff_CS = 0x11d0;
    DISPATCHMESSAGE(0x11d0,&stack0xffec);
  }
  return local_10;
}



/* ============================================================
   1008:1b22  FUN_1008_1b22
   ============================================================ */

void __stdcall16far
FUN_1008_1b22(int param_1,undefined2 param_2,int param_3,int param_4,int param_5)

{
  undefined2 *puVar1;
  int iVar2;
  char *pcVar3;
  undefined2 unaff_SS;
  char *pcVar4;
  char *pcVar5;
  char *pcVar6;
  undefined2 uVar7;
  undefined1 local_12a [294];
  int local_4;
  
  local_4 = 0;
  if (param_5 < 0) {
    *(undefined2 *)(param_1 + 0xee) = 1;
  }
  if (0 < param_5) {
    *(undefined2 *)(param_1 + 0xee) = 0;
  }
  iVar2 = FUN_1008_27b8(param_1,param_2,param_4,param_5,param_3);
  if (iVar2 != 0) {
    if (param_3 == 0) {
      FUN_1008_77d0(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde));
      FUN_1008_a060(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde),
                    *(undefined2 *)(param_1 + 0xec),*(undefined2 *)(param_1 + 0xea));
      puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 0x20);
      (*(code *)*puVar1)((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)(param_1 + 0xdc),
                         *(undefined2 *)(param_1 + 0xde),&param_4);
      *(int *)(param_1 + 0xea) = *(int *)(param_1 + 0xea) + param_5;
      *(int *)(param_1 + 0xec) = *(int *)(param_1 + 0xec) + param_4;
      FUN_1008_286c(param_1,param_2);
      DAT_1080_1070 = 1;
      FUN_1008_a060(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde),
                    *(undefined2 *)(param_1 + 0xec),*(undefined2 *)(param_1 + 0xea));
      puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 0x4c);
      (*(code *)*puVar1)((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)(param_1 + 0xdc),
                         *(undefined2 *)(param_1 + 0xde));
      FUN_1008_8310(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde));
      pcVar3 = (char *)0x11d0;
      UPDATEWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,
                   *(undefined2 *)((int)*(undefined4 *)(param_1 + 0xc) + 8));
      if ((param_5 != 0) || (param_4 != 0)) {
        pcVar3 = (char *)s_Lucky_Chip_1080_1004 + 4;
        FUN_1008_a0ca(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde),
                      *(undefined2 *)(param_1 + 0xec),*(undefined2 *)(param_1 + 0xea));
      }
    }
    else {
      if ((param_5 == 0) && (param_4 == 0)) goto LAB_1008_1d1c;
      pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
      pcVar4 = (char *)s_IN_THE_MURKY_CHAMBERS_beneath_yo_1080_1bf6 + 0x97;
      iVar2 = param_1;
      uVar7 = param_2;
      FUN_1008_1fe2(param_1,param_2,param_4,param_5,*(undefined2 *)(param_1 + 0xec),
                    *(undefined2 *)(param_1 + 0xea));
      FUN_1008_77d0(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde));
      puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 0x4c);
      pcVar6 = (char *)s_IN_THE_MURKY_CHAMBERS_beneath_yo_1080_1bf6 + 0xc2;
      (*(code *)*puVar1)((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)(param_1 + 0xdc),
                         *(undefined2 *)(param_1 + 0xde));
      FUN_1008_8310(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde));
      pcVar3 = (char *)0x11d0;
      UPDATEWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,
                   *(undefined2 *)((int)*(undefined4 *)(param_1 + 0xc) + 8),pcVar4,pcVar5,iVar2,
                   uVar7,pcVar6);
    }
    puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 0x1c);
    (*(code *)*puVar1)(pcVar3,*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde),
                       param_3,param_4,param_5);
    FUN_1008_8b2c(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde),
                  *(undefined2 *)((int)*(undefined4 *)(param_1 + 0xc) + 8));
    local_4 = 1;
  }
LAB_1008_1d1c:
  if (local_4 != 0) {
    puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(param_1 + 0xdc) + 0x44);
    (*(code *)*puVar1)((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)(param_1 + 0xdc),
                       *(undefined2 *)(param_1 + 0xde));
    FUN_1008_554e(*(undefined2 *)(param_1 + 0x10),*(undefined2 *)(param_1 + 0x12));
    if (*(int *)(param_1 + 0x100) == 1) {
      FUN_1008_2d02(param_1,param_2,0,0x353a,(char *)s_Lucky_Chip_1080_1004 + 4);
      FUN_1010_edea(local_12a,unaff_SS,param_1,param_2);
      FUN_1018_0252(local_12a,unaff_SS,*(undefined2 *)(param_1 + 0x100),
                    *(undefined2 *)(param_1 + 0xfe),*(undefined2 *)(param_1 + 0xe4),
                    *(undefined2 *)(param_1 + 0xf6),*(undefined2 *)(param_1 + 0xf8));
      FUN_1008_045e(param_1,param_2,*(undefined2 *)(param_1 + 0xd8),*(undefined2 *)(param_1 + 0xda))
      ;
      DAT_1080_1be6 = 0;
      FUN_1008_169c(param_1,param_2);
      FUN_1008_a5fa();
      DAT_1080_1070 = 1;
      FUN_1008_a652(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde));
      INVALIDATERECT((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,
                     *(undefined2 *)((int)*(undefined4 *)(param_1 + 8) + 4));
      FUN_1010_ef2e(local_12a,unaff_SS);
    }
    if ((*(int *)(param_1 + 0xf4) == 0) &&
       (*(int *)((int)*(undefined4 *)(param_1 + 0xdc) + 0xc) == 0)) {
      if ((*(int *)(param_1 + 0xf8) < 1) &&
         ((*(int *)(param_1 + 0xf8) < 0 || (*(uint *)(param_1 + 0xf6) < 1000)))) {
        uVar7 = 0x35d8;
      }
      else if ((*(int *)(param_1 + 0xf8) < 1) &&
              ((*(int *)(param_1 + 0xf8) < 0 || (*(uint *)(param_1 + 0xf6) < 20000)))) {
        uVar7 = 0x3654;
      }
      else {
        uVar7 = 14000;
      }
      FUN_1008_2d02(param_1,param_2,0,uVar7,(char *)s_Lucky_Chip_1080_1004 + 4);
      FUN_1010_edea(local_12a,unaff_SS,param_1,param_2);
      FUN_1018_0252(local_12a,unaff_SS,*(undefined2 *)(param_1 + 0x100),
                    *(undefined2 *)(param_1 + 0xfe),*(undefined2 *)(param_1 + 0xe4),
                    *(undefined2 *)(param_1 + 0xf6),*(undefined2 *)(param_1 + 0xf8));
      FUN_1008_045e(param_1,param_2,*(undefined2 *)(param_1 + 0xd8),*(undefined2 *)(param_1 + 0xda))
      ;
      DAT_1080_1be6 = 0;
      FUN_1008_169c(param_1,param_2);
      FUN_1008_a5fa();
      DAT_1080_1070 = 1;
      FUN_1008_a652(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde));
      INVALIDATERECT((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,
                     *(undefined2 *)((int)*(undefined4 *)(param_1 + 8) + 4));
      FUN_1010_ef2e(local_12a,unaff_SS);
    }
  }
  return;
}



/* ============================================================
   1008:1efe  FUN_1008_1efe
   ============================================================ */

void __stdcall16far FUN_1008_1efe(int param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 unaff_SS;
  undefined1 local_12a [296];
  
  iVar1 = FUN_1008_2d02(param_1,param_2,3,0x372e,(char *)s_Lucky_Chip_1080_1004 + 4);
  if (iVar1 == 6) {
    FUN_1008_134e(param_1,param_2);
  }
  else if (iVar1 != 7) goto LAB_1008_1fa7;
  if (iVar1 == 7) {
    FUN_1010_edea(local_12a,unaff_SS,param_1,param_2);
    FUN_1018_0252(local_12a,unaff_SS,*(undefined2 *)(param_1 + 0x100),
                  *(undefined2 *)(param_1 + 0xfe),*(undefined2 *)(param_1 + 0xe4),
                  *(undefined2 *)(param_1 + 0xf6),*(undefined2 *)(param_1 + 0xf8));
    FUN_1010_ef2e(local_12a,unaff_SS);
  }
  if (*(int *)(param_1 + 0xda) != 0 || *(int *)(param_1 + 0xd8) != 0) {
    FUN_1008_045e(param_1,param_2,*(undefined2 *)(param_1 + 0xd8),*(undefined2 *)(param_1 + 0xda));
  }
  DAT_1080_1be6 = 0;
  FUN_1008_169c(param_1,param_2);
LAB_1008_1fa7:
  FUN_1008_a5fa();
  DAT_1080_1070 = 1;
  FUN_1008_a652(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde));
  INVALIDATERECT((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,
                 *(undefined2 *)((int)*(undefined4 *)(param_1 + 8) + 4));
  return;
}



/* ============================================================
   1008:1fe2  FUN_1008_1fe2
   ============================================================ */

void __stdcall16far
FUN_1008_1fe2(undefined4 param_1,int param_2,int param_3,int param_4,int param_5)

{
  int *piVar1;
  undefined2 *puVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  undefined2 uVar10;
  int local_6;
  
  if (0x95 < *(int *)((int)*(undefined4 *)((int)param_1 + 0xdc) + 0x52)) {
    return;
  }
  local_6 = 1;
  uVar8 = *(undefined2 *)(*(int *)((int)param_1 + 0x104) * 4 + (int)param_1 + 0x106);
  uVar10 = *(undefined2 *)((int)param_1 + 0xde);
  uVar9 = *(undefined2 *)((int)param_1 + 0xdc);
  puVar2 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)((int)param_1 + 0xdc) + 0x24);
  iVar6 = param_2;
  iVar7 = param_3;
  iVar4 = (*(code *)*puVar2)();
  if ((iVar4 != 0) && (iVar4 != 1)) goto switchD_1008_2057_default;
  switch(*(undefined2 *)(*(int *)((int)param_1 + 0x104) * 4 + (int)param_1 + 0x106)) {
  case 0:
    uVar8 = 0;
    goto LAB_1008_208f;
  case 1:
    for (iVar6 = 0; iVar6 < 8; iVar6 = iVar6 + 1) {
      if ((*(int *)(iVar6 * 0xc + 0x16) == param_3) && (*(int *)(iVar6 * 0xc + 0x18) == param_2)) {
        iVar7 = iVar6 * 0xc;
        FUN_1008_8e22(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                      param_2,param_3,*(int *)(iVar7 + 0x1c) + param_4,
                      *(int *)(iVar7 + 0x1a) + param_5,0);
        FUN_1008_8e22(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                      param_2,param_3,param_4,param_5,0);
        FUN_1008_8e22(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                      param_2,param_3,*(int *)(iVar7 + 0x20) + param_4,
                      *(int *)(iVar7 + 0x1e) + param_5,0);
      }
    }
    uVar8 = 1;
    goto LAB_1008_2158;
  case 2:
    iVar6 = FUN_1008_917e(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                          param_2 + param_4,param_3 + param_5);
    if (((iVar6 != 0) &&
        (uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 0xdc) + 8),
        uVar3 = *(undefined4 *)((int)uVar3 + (param_3 + param_5) * 4),
        *(int *)((int)uVar3 + (param_2 + param_4) * 2) == 0)) &&
       (iVar6 = FUN_1008_a704(*(undefined2 *)((int)param_1 + 0xdc),
                              *(undefined2 *)((int)param_1 + 0xde),param_2 + param_4,
                              param_3 + param_5), iVar6 == 0)) {
      uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 0xdc) + 8);
      uVar3 = *(undefined4 *)((int)uVar3 + (param_3 + param_5) * 4);
      *(undefined2 *)((int)uVar3 + (param_2 + param_4) * 2) = 7;
      FUN_1008_a060(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                    param_2 + param_4,param_3 + param_5);
      goto LAB_1008_2214;
    }
    uVar8 = 0x37a4;
    goto LAB_1008_24d5;
  case 3:
    iVar6 = FUN_1008_917e(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                          param_2 + param_4,param_3 + param_5);
    if ((iVar6 != 0) &&
       (iVar6 = FUN_1008_a704(*(undefined2 *)((int)param_1 + 0xdc),
                              *(undefined2 *)((int)param_1 + 0xde),param_2 + param_4,
                              param_3 + param_5), iVar6 == 0)) {
      uVar8 = 1;
LAB_1008_2269:
      param_5 = param_3 + param_5;
      param_4 = param_2 + param_4;
      param_3 = 0;
      param_2 = 0;
      goto LAB_1008_227b;
    }
    uVar8 = 0x3788;
    goto LAB_1008_24d5;
  case 4:
    uVar8 = 1;
LAB_1008_227b:
    FUN_1008_8e22(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),param_2,
                  param_3,param_4,param_5,uVar8);
LAB_1008_2214:
    uVar8 = 2;
    goto LAB_1008_20a6;
  case 5:
    FUN_1008_8e22(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),param_2,
                  param_3,param_4,param_5,2);
    uVar8 = 4;
    goto LAB_1008_20a6;
  case 6:
    iVar6 = -1;
    do {
      for (iVar7 = -1; iVar7 < 2; iVar7 = iVar7 + 1) {
        if ((iVar6 != 0) || (iVar7 != 0)) {
          FUN_1008_8e22(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                        iVar7,iVar6,param_4,param_5,1);
        }
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < 2);
    goto LAB_1008_23b1;
  case 7:
    iVar6 = -1;
    do {
      for (iVar7 = -1; iVar7 < 2; iVar7 = iVar7 + 1) {
        if ((iVar6 != 0) || (iVar7 != 0)) {
          FUN_1008_8e22(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                        iVar7,iVar6,param_4,param_5,2);
        }
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < 2);
    uVar8 = 5;
    goto LAB_1008_2158;
  case 8:
    iVar6 = FUN_1008_917e(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                          param_2 + param_4,param_3 + param_5);
    if (iVar6 != 0) {
      uVar8 = 3;
      goto LAB_1008_2269;
    }
    uVar8 = 0x3766;
    goto LAB_1008_24d5;
  case 9:
    FUN_1008_a732(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde));
    break;
  case 10:
    for (iVar6 = 0; iVar6 < 10; iVar6 = iVar6 + 1) {
      FUN_1008_8e22(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                    param_2,param_3,param_4,param_5,1);
    }
LAB_1008_23b1:
    uVar8 = 3;
LAB_1008_2158:
    FUN_1008_31d4((int)param_1,param_1._2_2_,uVar8);
    break;
  case 0xb:
    uVar8 = 4;
LAB_1008_208f:
    FUN_1008_8e22(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),param_2,
                  param_3,param_4,param_5,uVar8);
    uVar8 = 0;
    goto LAB_1008_20a6;
  case 0xc:
    *(undefined2 *)((int)param_1 + 0xfa) = 1;
    *(undefined2 *)((int)param_1 + 0xfc) = 100;
    break;
  case 0xd:
    puVar2 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)((int)param_1 + 0xdc) + 0x48);
    (*(code *)*puVar2)((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)((int)param_1 + 0xdc),
                       *(undefined2 *)((int)param_1 + 0xde));
    break;
  case 0xe:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x14:
switchD_1008_2057_caseD_e:
    uVar8 = 0x3754;
LAB_1008_24d5:
    FUN_1008_2822((int)param_1,param_1._2_2_,uVar8,(char *)s_Lucky_Chip_1080_1004 + 4);
    local_6 = 0;
    break;
  case 0xf:
    FUN_1008_2c08((int)param_1,param_1._2_2_);
    break;
  case 0x13:
    iVar5 = FUN_1008_91c6(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                          param_4 + param_2,param_3 + param_5);
    if (iVar5 == 0) {
      return;
    }
    uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 0xdc) + 8);
    uVar3 = *(undefined4 *)((int)uVar3 + (param_3 + param_5) * 4);
    if ((0x7e < *(int *)((int)uVar3 + (param_4 + param_2) * 2)) &&
       (uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 0xdc) + 8),
       uVar3 = *(undefined4 *)((int)uVar3 + (param_3 + param_5) * 4),
       *(int *)((int)uVar3 + (param_4 + param_2) * 2) < 0x83)) {
      puVar2 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)((int)param_1 + 0xdc) + 0x38);
      local_6 = (*(code *)*puVar2)((char *)s_Lucky_Chip_1080_1004 + 4,
                                   *(undefined2 *)((int)param_1 + 0xdc),
                                   *(undefined2 *)((int)param_1 + 0xde),param_4 + param_2,
                                   param_3 + param_5,0x13);
      break;
    }
    uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 0xdc) + 8);
    uVar3 = *(undefined4 *)((int)uVar3 + (param_3 + param_5) * 4);
    if (*(int *)((int)uVar3 + (param_4 + param_2) * 2) != 0x5c) goto switchD_1008_2057_caseD_e;
    iVar6 = FUN_1008_3c88(4,uVar9,uVar10,uVar8,iVar6,iVar7);
    if (iVar6 == 1) {
      uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 0xdc) + 8);
      uVar3 = *(undefined4 *)((int)uVar3 + (param_3 + param_5) * 4);
      *(undefined2 *)((int)uVar3 + (param_4 + param_2) * 2) = 0;
      uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0xdc) >> 0x10);
      iVar6 = (int)*(undefined4 *)((int)param_1 + 0xdc);
      FUN_1008_af5a(*(undefined2 *)(iVar6 + 0x36),*(undefined2 *)(iVar6 + 0x38),0x1d,
                    param_4 + param_2,param_3 + param_5);
      FUN_1008_2822((int)param_1,param_1._2_2_,0x37c4,(char *)s_Lucky_Chip_1080_1004 + 4);
      local_6 = 0;
    }
    else if (((0 < iVar6 + -1) && (!SBORROW2(iVar6 + -1,1))) && (iVar6 + -3 < 2)) {
      uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 0xdc) + 8);
      uVar3 = *(undefined4 *)((int)uVar3 + (param_3 + param_5) * 4);
      *(undefined2 *)((int)uVar3 + (param_2 + param_4) * 2) = 0;
      FUN_1008_2822((int)param_1,param_1._2_2_,0x37ec,(char *)s_Lucky_Chip_1080_1004 + 4);
      uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0xdc) >> 0x10);
      iVar6 = (int)*(undefined4 *)((int)param_1 + 0xdc);
      FUN_1008_af5a(*(undefined2 *)(iVar6 + 0x36),*(undefined2 *)(iVar6 + 0x38),0x1d,
                    param_2 + param_4,param_3 + param_5);
    }
    uVar8 = 0x12;
LAB_1008_20a6:
    FUN_1008_31d4((int)param_1,param_1._2_2_,uVar8);
  }
switchD_1008_2057_default:
  if (((iVar4 == 0) || (iVar4 == 2)) &&
     ((local_6 != 0 &&
      ((*(int *)(*(int *)((int)param_1 + 0x104) * 4 + (int)param_1 + 0x106) != 0 &&
       (*(int *)(*(int *)((int)param_1 + 0x104) * 4 + (int)param_1 + 0x106) != 1)))))) {
    piVar1 = (int *)(*(int *)((int)param_1 + 0x104) * 4 + (int)param_1 + 0x108);
    *piVar1 = *piVar1 + -1;
    if (*(int *)(*(int *)((int)param_1 + 0x104) * 4 + (int)param_1 + 0x108) < 1) {
      FUN_1008_2766((int)param_1,param_1._2_2_,*(undefined2 *)((int)param_1 + 0x104));
      iVar6 = FUN_1008_29b2((int)param_1,param_1._2_2_,1);
      if (iVar6 == 0) {
        *(undefined2 *)((int)param_1 + 0x104) = 0;
LAB_1008_26cb:
        *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) = 0;
      }
      else {
        for (iVar6 = 0; iVar6 < *(int *)((int)param_1 + 0x102); iVar6 = iVar6 + 1) {
          if (*(int *)(iVar6 * 4 + (int)param_1 + 0x106) == 1) {
            *(int *)((int)param_1 + 0x104) = iVar6;
            if ((iVar6 < *(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68)) ||
               (*(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) + 4 < iVar6)) {
              if (*(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) + 4 < iVar6) {
                *(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) = iVar6 + -4;
                if (*(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) < 0)
                goto LAB_1008_26cb;
              }
              else {
                *(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) = iVar6;
              }
            }
            break;
          }
        }
      }
    }
    FUN_1008_5a8a(*(undefined2 *)((int)param_1 + 0x10),*(undefined2 *)((int)param_1 + 0x12));
  }
  return;
}



/* ============================================================
   1008:2766  FUN_1008_2766
   ============================================================ */

void __stdcall16far FUN_1008_2766(int param_1,undefined2 param_2,int param_3)

{
  undefined2 uVar1;
  int iVar2;
  
  for (; param_3 < *(int *)(param_1 + 0x102) + -1; param_3 = param_3 + 1) {
    iVar2 = param_3 * 4 + param_1;
    uVar1 = *(undefined2 *)(iVar2 + 0x10c);
    *(undefined2 *)(iVar2 + 0x106) = *(undefined2 *)(iVar2 + 0x10a);
    *(undefined2 *)(iVar2 + 0x108) = uVar1;
  }
  *(int *)(param_1 + 0x102) = *(int *)(param_1 + 0x102) + -1;
  FUN_1008_5a8a(*(undefined2 *)(param_1 + 0x10),*(undefined2 *)(param_1 + 0x12));
  return;
}



/* ============================================================
   1008:27b8  FUN_1008_27b8
   ============================================================ */

bool __stdcall16far FUN_1008_27b8(undefined4 param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  bool bVar2;
  
  if (param_4 == 0) {
    iVar1 = FUN_1008_91c6(*(undefined2 *)((int)param_1 + 0xdc),*(undefined2 *)((int)param_1 + 0xde),
                          *(int *)((int)param_1 + 0xec) + param_2,
                          *(int *)((int)param_1 + 0xea) + param_3);
    bVar2 = false;
    if (iVar1 != 0) {
      iVar1 = FUN_1008_917e(*(undefined2 *)((int)param_1 + 0xdc),
                            *(undefined2 *)((int)param_1 + 0xde),
                            *(int *)((int)param_1 + 0xec) + param_2,
                            param_3 + *(int *)((int)param_1 + 0xea));
      bVar2 = iVar1 != 0;
    }
  }
  else {
    bVar2 = true;
  }
  return bVar2;
}



/* ============================================================
   1008:2822  FUN_1008_2822
   ============================================================ */

void __stdcall16far FUN_1008_2822(undefined4 param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  
  uVar1 = (undefined2)((ulong)param_1 >> 0x10);
  FUN_1008_55e0(*(undefined2 *)((int)param_1 + 0x10),*(undefined2 *)((int)param_1 + 0x12),param_2,
                param_3);
  return;
}



/* ============================================================
   1008:2840  FUN_1008_2840
   ============================================================ */

void __stdcall16far FUN_1008_2840(int param_1,undefined2 param_2,uint param_3,int param_4)

{
  uint *puVar1;
  uint uVar2;
  
  puVar1 = (uint *)(param_1 + 0xf6);
  uVar2 = *puVar1;
  *puVar1 = *puVar1 + param_3;
  *(int *)(param_1 + 0xf8) = *(int *)(param_1 + 0xf8) + param_4 + (uint)CARRY2(uVar2,param_3);
  FUN_1008_5ac8(*(undefined2 *)(param_1 + 0x10),*(undefined2 *)(param_1 + 0x12));
  return;
}



/* ============================================================
   1008:286c  FUN_1008_286c
   ============================================================ */

undefined2 __stdcall16far FUN_1008_286c(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  
  uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0xdc) >> 0x10);
  iVar2 = (int)*(undefined4 *)(param_1 + 0xdc);
  iVar1 = *(int *)(iVar2 + 0x2c);
  uVar4 = *(undefined2 *)(iVar2 + 0x2e);
  for (iVar2 = 0; iVar2 < *(int *)((int)*(undefined4 *)(param_1 + 0xdc) + 0x50); iVar2 = iVar2 + 1)
  {
    if ((*(int *)(iVar2 * 0xc + iVar1 + 2) == *(int *)(param_1 + 0xea)) &&
       (*(int *)(iVar2 * 0xc + iVar1 + 4) == *(int *)(param_1 + 0xec))) {
      FUN_1008_2920(param_1,param_2,*(undefined2 *)(iVar2 * 0xc + iVar1));
      uVar5 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0xdc) >> 0x10);
      iVar3 = (int)*(undefined4 *)(param_1 + 0xdc);
      FUN_1008_af5a(*(undefined2 *)(iVar3 + 0x36),*(undefined2 *)(iVar3 + 0x38),0x18,
                    *(undefined2 *)(param_1 + 0xec),*(undefined2 *)(param_1 + 0xea));
      FUN_1008_8032(*(undefined2 *)(param_1 + 0xdc),*(undefined2 *)(param_1 + 0xde),iVar2);
      iVar2 = iVar2 + -1;
    }
  }
  return 1;
}



/* ============================================================
   1008:2920  FUN_1008_2920
   ============================================================ */

void __stdcall16far FUN_1008_2920(undefined4 param_1,int param_2)

{
  if (param_2 < 0) goto LAB_1008_2960;
  if ((param_2 < 2) || (param_2 == 2)) {
LAB_1008_293e:
    *(int *)((int)param_1 + 0xf0) = *(int *)((int)param_1 + 0xf0) + -1;
  }
  else {
    if (param_2 != 3) {
      if (param_2 != 4) goto LAB_1008_2960;
      goto LAB_1008_293e;
    }
    *(int *)((int)param_1 + 0xf0) = *(int *)((int)param_1 + 0xf0) + -2;
  }
  if (*(int *)((int)param_1 + 0xf0) < 0) {
    *(undefined2 *)((int)param_1 + 0xf0) = 0;
  }
LAB_1008_2960:
  FUN_1008_31d4((int)param_1,param_1._2_2_,0x1c);
  FUN_1008_5ac8(*(undefined2 *)((int)param_1 + 0x10),*(undefined2 *)((int)param_1 + 0x12));
  if (((*(int *)((int)param_1 + 0xf0) == 0) && (*(int *)((int)param_1 + 0xe2) == 0)) &&
     (*(int *)((int)*(undefined4 *)((int)param_1 + 0xdc) + 0xc) == 0)) {
    *(undefined2 *)((int)param_1 + 0xf4) = 0;
    FUN_1008_31d4((int)param_1,param_1._2_2_,0x1e);
  }
  return;
}



/* ============================================================
   1008:29b2  FUN_1008_29b2
   ============================================================ */

undefined2 __stdcall16far FUN_1008_29b2(int param_1,undefined2 param_2,int param_3)

{
  int iVar1;
  
  iVar1 = 0;
  while( true ) {
    if (*(int *)(param_1 + 0x102) <= iVar1) {
      return 0;
    }
    if (*(int *)(iVar1 * 4 + param_1 + 0x106) == param_3) break;
    iVar1 = iVar1 + 1;
  }
  return 1;
}



/* ============================================================
   1008:29e8  FUN_1008_29e8
   ============================================================ */

void __stdcall16far FUN_1008_29e8(undefined4 param_1)

{
  int iVar1;
  undefined2 uVar2;
  
  uVar2 = (undefined2)((ulong)param_1 >> 0x10);
  iVar1 = (int)param_1;
  if (0 < *(int *)(iVar1 + 0x104)) {
    *(int *)(iVar1 + 0x104) = *(int *)(iVar1 + 0x104) + -1;
    FUN_1008_5a8a(*(undefined2 *)(iVar1 + 0x10),*(undefined2 *)(iVar1 + 0x12));
    if (*(int *)(iVar1 + 0x104) < *(int *)((int)*(undefined4 *)(iVar1 + 0x10) + 0x68)) {
      FUN_1008_5a04(*(undefined2 *)(iVar1 + 0x10),*(undefined2 *)(iVar1 + 0x12),1);
    }
  }
  if ((*(int *)(iVar1 + 0x104) < *(int *)((int)*(undefined4 *)(iVar1 + 0x10) + 0x68)) ||
     (*(int *)((int)*(undefined4 *)(iVar1 + 0x10) + 0x68) + 5 <= *(int *)(iVar1 + 0x104))) {
    *(int *)((int)*(undefined4 *)(iVar1 + 0x10) + 0x68) = *(int *)(iVar1 + 0x104) + -4;
    if (*(int *)((int)*(undefined4 *)(iVar1 + 0x10) + 0x68) < 0) {
      *(undefined2 *)((int)*(undefined4 *)(iVar1 + 0x10) + 0x68) = 0;
    }
    FUN_1008_5a8a(*(undefined2 *)(iVar1 + 0x10),*(undefined2 *)(iVar1 + 0x12));
  }
  return;
}



/* ============================================================
   1008:2a98  FUN_1008_2a98
   ============================================================ */

void __stdcall16far FUN_1008_2a98(undefined4 param_1)

{
  if (*(int *)((int)param_1 + 0x104) < *(int *)((int)param_1 + 0x102) + -1) {
    *(int *)((int)param_1 + 0x104) = *(int *)((int)param_1 + 0x104) + 1;
    FUN_1008_5a8a(*(undefined2 *)((int)param_1 + 0x10),*(undefined2 *)((int)param_1 + 0x12));
    if (*(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) + 5 <=
        *(int *)((int)param_1 + 0x104)) {
      FUN_1008_5a04(*(undefined2 *)((int)param_1 + 0x10),*(undefined2 *)((int)param_1 + 0x12),0);
    }
  }
  if ((*(int *)((int)param_1 + 0x104) < *(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68))
     || (*(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) + 5 <=
         *(int *)((int)param_1 + 0x104))) {
    *(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) = *(int *)((int)param_1 + 0x104) + -4
    ;
    if (*(int *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) < 0) {
      *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 0x10) + 0x68) = 0;
    }
    FUN_1008_5a8a(*(undefined2 *)((int)param_1 + 0x10),*(undefined2 *)((int)param_1 + 0x12));
  }
  return;
}



/* ============================================================
   1008:2c08  FUN_1008_2c08
   ============================================================ */

void __stdcall16far FUN_1008_2c08(int param_1,undefined2 param_2)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  
  FUN_1008_2822(param_1,param_2,0x37fc,(char *)s_Lucky_Chip_1080_1004 + 4);
  iVar2 = FUN_1008_3c88(0x2f);
  iVar2 = iVar2 + -1;
  FUN_1008_2822(param_1,param_2,iVar2 * 0x50,0x1020);
  bVar1 = false;
  for (iVar3 = 0; iVar3 < *(int *)(param_1 + 0xe6); iVar3 = iVar3 + 1) {
    if (*(int *)(iVar3 * 2 + param_1 + 0x1ce) == iVar2) {
      bVar1 = true;
    }
  }
  if (!bVar1) {
    *(int *)(*(int *)(param_1 + 0xe6) * 2 + param_1 + 0x1ce) = iVar2;
    *(int *)(param_1 + 0xe6) = *(int *)(param_1 + 0xe6) + 1;
  }
  return;
}



/* ============================================================
   1008:2c84  FUN_1008_2c84
   ============================================================ */

void __stdcall16far FUN_1008_2c84(undefined4 param_1)

{
  int iVar1;
  undefined2 uVar2;
  char *unaff_SS;
  undefined1 *puVar3;
  undefined1 local_5c [90];
  
  uVar2 = (undefined2)((ulong)param_1 >> 0x10);
  iVar1 = (int)param_1;
  if (*(int *)(iVar1 + 0xe8) == 0) {
    *(undefined2 *)(iVar1 + 0xe8) = *(undefined2 *)(iVar1 + 0xe6);
  }
  if (*(int *)(iVar1 + 0xe8) == 0) {
    unaff_SS = (char *)s_Lucky_Chip_1080_1004 + 4;
    puVar3 = (undefined1 *)0x3820;
  }
  else {
    *(int *)(iVar1 + 0xe8) = *(int *)(iVar1 + 0xe8) + -1;
    FUN_1000_1924(local_5c);
    puVar3 = local_5c;
  }
  FUN_1008_67e6(*(undefined2 *)(iVar1 + 0xc),*(undefined2 *)(iVar1 + 0xe),puVar3,unaff_SS);
  return;
}



/* ============================================================
   1008:2d02  FUN_1008_2d02
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_2d02(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  undefined2 uVar1;
  undefined2 unaff_CS;
  undefined2 uVar2;
  
  *(undefined2 *)(param_1 + 6) = 1;
  uVar2 = *(undefined2 *)((int)*(undefined4 *)(param_1 + 8) + 4);
  uVar1 = MESSAGEBOX(unaff_CS,param_3,0x384e,(char *)s_Lucky_Chip_1080_1004 + 4,param_4,param_5);
  *(undefined2 *)(param_1 + 6) = 0;
  FUN_1008_a5fa(uVar2);
  return uVar1;
}



/* ============================================================
   1008:2d50  FUN_1008_2d50
   ============================================================ */

int __stdcall16far FUN_1008_2d50(undefined4 param_1)

{
  char extraout_AH;
  uint uVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar3 = (undefined2)((ulong)param_1 >> 0x10);
  iVar2 = (int)param_1;
  uVar1 = -(uint)(*(int *)(iVar2 + 0xfa) != 0) & 0x18;
  if ((*(int *)(iVar2 + 0xf4) == 0) && (*(int *)(iVar2 + 0xe2) == 0)) {
    iVar2 = DAT_1080_14ce % 3 + 0x3e;
  }
  else {
    GETASYNCKEYSTATE();
    if (extraout_AH == '\0') {
      DAT_1080_0076 = 0;
      if (*(int *)(iVar2 + 0xee) == 0) {
        iVar2 = DAT_1080_14ce % 8 + uVar1 + 0x152;
      }
      else {
        iVar2 = DAT_1080_14ce % 8 + uVar1 + 0x15a;
      }
    }
    else {
      if (*(int *)(iVar2 + 0xee) == 0) {
        iVar2 = DAT_1080_0076 + 0x166;
      }
      else {
        iVar2 = DAT_1080_0076 + 0x162;
      }
      DAT_1080_0076 = DAT_1080_0076 + 1;
      if (3 < DAT_1080_0076) {
        DAT_1080_0076 = 3;
      }
    }
  }
  return iVar2;
}



/* ============================================================
   1008:2dfc  FUN_1008_2dfc
   ============================================================ */

void __stdcall16far FUN_1008_2dfc(undefined2 *param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  int in_DX;
  undefined4 uVar3;
  
  iVar2 = FUN_1000_1646(0x1e);
  if (in_DX == 0 && iVar2 == 0) {
    param_1[0x6a] = 0;
    param_1[0x69] = 0;
  }
  else {
    uVar3 = FUN_1010_9572(iVar2,in_DX,*param_1,
                          *(undefined2 *)((int)*(undefined4 *)(param_1 + 4) + 4),param_1,param_2);
    param_1[0x69] = (int)uVar3;
    param_1[0x6a] = (int)((ulong)uVar3 >> 0x10);
  }
  if (param_1[0x6a] != 0 || param_1[0x69] != 0) {
    FUN_1010_96fa(param_1[0x69],param_1[0x6a]);
    iVar2 = param_1[0x69];
    iVar1 = param_1[0x6a];
    if (iVar1 != 0 || iVar2 != 0) {
      FUN_1010_9694(iVar2,iVar1);
      FUN_1000_1634(iVar2,iVar1);
    }
    param_1[0x6a] = 0;
    param_1[0x69] = 0;
  }
  return;
}



/* ============================================================
   1008:2eba  FUN_1008_2eba
   ============================================================ */

undefined2 __stdcall16far FUN_1008_2eba(int param_1,undefined2 param_2)

{
  undefined2 uVar1;
  int iVar2;
  undefined2 unaff_CS;
  char *in_stack_0000ffee;
  char *pcVar3;
  int local_6;
  int local_4;
  
  local_4 = 0;
  do {
    if (local_4 == 0x1f) {
      return 1;
    }
    uVar1 = unaff_CS;
    switch(local_4) {
    case 0:
      uVar1 = 0x3860;
      break;
    case 1:
      uVar1 = 0x386a;
      break;
    case 2:
      uVar1 = 0x3876;
      break;
    case 3:
      uVar1 = 0x3884;
      break;
    case 4:
      uVar1 = 0x3894;
      break;
    case 5:
      uVar1 = 0x38a6;
      break;
    case 6:
      uVar1 = 0x38ba;
      break;
    case 7:
      uVar1 = 0x38c8;
      break;
    case 8:
      uVar1 = 0x38d6;
      break;
    case 9:
      uVar1 = 0x38e4;
      break;
    case 10:
      uVar1 = 0x38ec;
      break;
    case 0xb:
      uVar1 = 0x38fa;
      break;
    case 0xc:
      uVar1 = 0x3908;
      break;
    case 0xd:
      uVar1 = 0x399a;
      break;
    case 0xe:
      uVar1 = 0x39a6;
      break;
    case 0xf:
      uVar1 = 0x39b4;
      break;
    case 0x10:
      uVar1 = 0x39c2;
      break;
    case 0x11:
      uVar1 = 0x39d0;
      break;
    case 0x12:
      uVar1 = 0x39e2;
      break;
    case 0x13:
      uVar1 = 0x39ee;
      break;
    case 0x14:
      uVar1 = 0x393a;
      break;
    case 0x15:
      uVar1 = 0x3948;
      break;
    case 0x16:
      uVar1 = 0x3a00;
      break;
    case 0x17:
      uVar1 = 0x392c;
      break;
    case 0x18:
      uVar1 = 0x3914;
      break;
    case 0x19:
      uVar1 = 0x3920;
      break;
    case 0x1a:
      uVar1 = 0x3960;
      break;
    case 0x1b:
      uVar1 = 0x3956;
      break;
    case 0x1c:
      uVar1 = 0x3968;
      break;
    case 0x1d:
      uVar1 = 0x3976;
      break;
    case 0x1e:
      uVar1 = 0x398a;
      break;
    default:
      goto switchD_1008_2ee2_default;
    }
    in_stack_0000ffee = (char *)s_Lucky_Chip_1080_1004 + 4;
    local_6 = FINDRESOURCE((char *)s_Lucky_Chip_1080_1004 + 4,0x385a,
                           (char *)s_Lucky_Chip_1080_1004 + 4,uVar1,
                           (char *)s_Lucky_Chip_1080_1004 + 4);
    uVar1 = 0x11d0;
switchD_1008_2ee2_default:
    if (local_6 == 0) {
      pcVar3 = (char *)0x3a10;
LAB_1008_3147:
      MESSAGEBOX(uVar1,0,0x3a0a,(char *)s_Lucky_Chip_1080_1004 + 4,in_stack_0000ffee,pcVar3);
      return 0;
    }
    unaff_CS = 0x11d0;
    in_stack_0000ffee = (char *)0x3107;
    uVar1 = LOADRESOURCE(uVar1,local_6);
    *(undefined2 *)(param_1 + local_4 * 2 + 0x14) = uVar1;
    if (*(int *)(param_1 + 0x14 + local_4 * 2) == 0) {
      pcVar3 = (char *)s_Lucky_Chip_1080_1004 + 4;
      in_stack_0000ffee = (char *)0x3a32;
      uVar1 = unaff_CS;
      goto LAB_1008_3147;
    }
    iVar2 = local_4 * 4 + param_1;
    *(undefined2 *)(iVar2 + 0x54) = 0;
    *(undefined2 *)(iVar2 + 0x52) = 0;
    local_4 = local_4 + 1;
  } while( true );
}



/* ============================================================
   1008:316c  FUN_1008_316c
   ============================================================ */

undefined2 __stdcall16far FUN_1008_316c(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  undefined2 unaff_CS;
  undefined2 uVar3;
  
  uVar3 = 0;
  SNDPLAYSOUND(unaff_CS,0,0);
  for (iVar2 = 0; iVar2 != 0x1f; iVar2 = iVar2 + 1) {
    iVar1 = iVar2 * 4 + param_1;
    if (*(int *)(iVar1 + 0x54) != 0 || *(int *)(iVar1 + 0x52) != 0) {
      GLOBALUNLOCK(0x11d0,*(undefined2 *)(iVar2 * 2 + param_1 + 0x14),uVar3);
    }
    if (*(int *)(iVar2 * 2 + param_1 + 0x14) != 0) {
      FREERESOURCE(0x11d0,*(undefined2 *)(iVar2 * 2 + param_1 + 0x14));
    }
  }
  return 1;
}



/* ============================================================
   1008:31d4  FUN_1008_31d4
   ============================================================ */

undefined2 __stdcall16far FUN_1008_31d4(undefined4 param_1,int param_2)

{
  undefined2 uVar1;
  int iVar2;
  undefined2 unaff_CS;
  undefined4 uVar3;
  
  iVar2 = DAT_1080_0078;
  if ((*(int *)((int)param_1 + 0xe0) == 0) || (iVar2 = param_2, param_2 == 0x20)) {
    DAT_1080_0078 = iVar2;
    uVar1 = 1;
  }
  else {
    if ((param_2 == 9) && (DAT_1080_0078 == 9)) {
      param_2 = 10;
    }
    else if ((param_2 == 9) && (DAT_1080_0078 == 10)) {
      param_2 = 0xb;
    }
    else if ((param_2 == 0x1c) && (DAT_1080_0078 == 0x1c)) {
      param_2 = 0x1d;
    }
    else if ((param_2 == 6) && (DAT_1080_0078 == 6)) {
      param_2 = 7;
    }
    else if ((param_2 == 6) && (DAT_1080_0078 == 7)) {
      param_2 = 8;
    }
    if ((DAT_1080_0078 < param_2) || (DAT_1080_0078 == 0x20)) {
      iVar2 = param_2 * 4 + (int)param_1;
      DAT_1080_0078 = param_2;
      if (*(int *)(iVar2 + 0x54) == 0 && *(int *)(iVar2 + 0x52) == 0) {
        unaff_CS = 0x11d0;
        uVar3 = LOCKRESOURCE();
        *(undefined2 *)((int)param_1 + param_2 * 4 + 0x52) = (int)uVar3;
        *(undefined2 *)((int)param_1 + param_2 * 4 + 0x54) = (int)((ulong)uVar3 >> 0x10);
      }
      if (*(int *)((int)param_1 + 0x52 + param_2 * 4 + 2) != 0 ||
          *(int *)((int)param_1 + 0x52 + param_2 * 4) != 0) {
        uVar1 = SNDPLAYSOUND(unaff_CS,7,*(undefined2 *)((int)param_1 + 0x52 + param_2 * 4));
        return uVar1;
      }
      FUN_1008_2d02((int)param_1,param_1._2_2_,0,0x3a58,(char *)s_Lucky_Chip_1080_1004 + 4);
    }
    uVar1 = 0;
  }
  return uVar1;
}



/* ============================================================
   1008:3a6e  FUN_1008_3a6e
   ============================================================ */

bool __cdecl16far FUN_1008_3a6e(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined2 unaff_SS;
  char local_12e [300];
  
  FUN_1000_16aa(local_12e);
  FUN_1000_1656(local_12e);
  FUN_1000_1656(local_12e);
  FUN_1000_1656(local_12e);
  FUN_1000_1656(local_12e);
  FUN_1000_1656(local_12e);
  FUN_1000_1656(local_12e);
  FUN_1000_1656(local_12e);
  FUN_1000_1656(local_12e);
  FUN_1000_1656(local_12e);
  FUN_1000_1656(local_12e);
  uVar3 = 0xff;
  for (iVar2 = 0; iVar1 = FUN_1000_1710(local_12e), iVar2 < iVar1; iVar2 = iVar2 + 1) {
    uVar3 = uVar3 ^ (int)local_12e[iVar2];
  }
  return uVar3 == 0xe4;
}



/* ============================================================
   1008:3b6c  FUN_1008_3b6c
   ============================================================ */

undefined2 * __stdcall16far FUN_1008_3b6c(undefined2 *param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  
  param_1[2] = param_3;
  uVar1 = CREATECOMPATIBLEDC();
  *param_1 = uVar1;
  uVar1 = SELECTOBJECT(0x11d0,DAT_1080_298a);
  param_1[1] = uVar1;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1008:3ba6  FUN_1008_3ba6
   ============================================================ */

void __stdcall16far FUN_1008_3ba6(undefined2 *param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined2 uVar1;
  
  uVar1 = *param_1;
  SELECTOBJECT(unaff_CS,param_1[1]);
  DELETEDC(0x11d0,*param_1,uVar1);
  return;
}



/* ============================================================
   1008:3bd0  FUN_1008_3bd0
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_3bd0(undefined2 *param_1,undefined2 param_2,int param_3,int param_4,int param_5)

{
  undefined2 unaff_CS;
  
  param_5 = param_5 * 10;
  BITBLT(unaff_CS,0x20,0xcc,*(int *)(param_5 + 4) * 0xc,*(int *)(param_5 + 2) * 0xc,*param_1,
         *(int *)(param_5 + 8) * 0xc,*(int *)(param_5 + 6) * 0xc,param_3 * 0xc,param_4 * 0xc);
  return 1;
}



/* ============================================================
   1008:3c2e  FUN_1008_3c2e
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_3c2e(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             int param_5)

{
  undefined2 unaff_CS;
  
  param_5 = param_5 * 10;
  BITBLT(unaff_CS,0x20,0xcc,*(int *)(param_5 + 4) * 0xc,*(int *)(param_5 + 2) * 0xc,*param_1,
         *(int *)(param_5 + 8) * 0xc,*(int *)(param_5 + 6) * 0xc,param_3,param_4);
  return 1;
}



/* ============================================================
   1008:3c88  FUN_1008_3c88
   ============================================================ */

int __cdecl16far FUN_1008_3c88(int param_1)

{
  int iVar1;
  undefined2 unaff_CS;
  
  if (param_1 < 1) {
    MESSAGEBOX(unaff_CS,0,0x336c,(char *)s_Lucky_Chip_1080_1004 + 4,0x44f6,
               (char *)s_Lucky_Chip_1080_1004 + 4);
    return 0;
  }
  iVar1 = FUN_1000_1f6e();
  return iVar1 % param_1 + 1;
}



/* ============================================================
   1008:3cc2  FUN_1008_3cc2
   ============================================================ */

void __cdecl16far
FUN_1008_3cc2(undefined2 param_1,int param_2,int param_3,undefined2 param_4,undefined2 param_5)

{
  undefined2 uVar1;
  undefined4 uVar2;
  
  uVar1 = FUN_1000_1710(param_4,param_5);
  uVar2 = SETTEXTCOLOR(0x1000,0xffff,0xff);
  TEXTOUT(0x11d0,uVar1,param_4,param_5,param_3 + -1,param_2 + -1);
  SETTEXTCOLOR(0x11d0,0,0);
  SETBKMODE(0x11d0,1);
  TEXTOUT(0x11d0,uVar1,param_4,param_5,param_3 + 1,param_2 + 1);
  SETTEXTCOLOR(0x11d0,uVar2);
  TEXTOUT(0x11d0,uVar1,param_4,param_5,param_3,param_2);
  return;
}



/* ============================================================
   1008:3d50  FUN_1008_3d50
   ============================================================ */

void __cdecl16far
FUN_1008_3d50(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  undefined2 unaff_CS;
  undefined2 uVar1;
  
  uVar1 = param_1;
  SETBKMODE(unaff_CS,1);
  FUN_1008_3cc2(param_1,param_2,param_3,param_4,param_5,uVar1);
  return;
}



/* ============================================================
   1008:3f18  FUN_1008_3f18
   ============================================================ */

void __cdecl16far FUN_1008_3f18(undefined2 param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined4 uVar1;
  char *pcVar2;
  undefined2 uVar3;
  
  uVar3 = 0x4518;
  pcVar2 = (char *)s_Lucky_Chip_1080_1004 + 4;
  uVar1 = MAKEPROCINSTANCE(unaff_CS,param_2,0x3d78);
  DIALOGBOX(0x11d0,uVar1,pcVar2,param_1,uVar3);
  FREEPROCINSTANCE(0x11d0,(int)uVar1);
  return;
}



/* ============================================================
   1008:3fd6  FUN_1008_3fd6
   ============================================================ */

undefined2 __cdecl16far FUN_1008_3fd6(undefined2 param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined4 uVar1;
  char *pcVar2;
  undefined2 uVar3;
  
  DAT_1080_298c = 1;
  uVar3 = 0x4522;
  pcVar2 = (char *)s_Lucky_Chip_1080_1004 + 4;
  uVar1 = MAKEPROCINSTANCE(unaff_CS,param_2,0x3f4e);
  DIALOGBOX(0x11d0,uVar1,pcVar2,param_1,uVar3);
  FREEPROCINSTANCE(0x11d0,(int)uVar1);
  return DAT_1080_298c;
}



/* ============================================================
   1008:4014  FUN_1008_4014
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_4014(undefined2 param_1,undefined2 param_2,int param_3,int param_4,undefined2 param_5)

{
  undefined2 uVar1;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  
  if (param_4 == 2) {
LAB_1008_4031:
    ENDDIALOG(unaff_CS,0);
    uVar1 = 1;
  }
  else {
    if (param_4 == 0x110) {
      uVar1 = GETDLGITEM(unaff_CS,1);
      SETFOCUS(0x11d0,uVar1,param_5);
    }
    else if ((param_4 == 0x111) && (param_3 == 100)) {
      uVar1 = GETDLGITEM(unaff_CS,1);
      unaff_CS = 0x11d0;
      GETWINDOWTEXT(0x11d0,0xd,*(undefined2 *)&DAT_1080_298e,*(undefined2 *)&DAT_1080_2990,uVar1,
                    param_5);
      goto LAB_1008_4031;
    }
    uVar1 = 0;
  }
  return uVar1;
}



/* ============================================================
   1008:407e  FUN_1008_407e
   ============================================================ */

void __cdecl16far
FUN_1008_407e(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  undefined2 unaff_CS;
  undefined4 uVar1;
  char *pcVar2;
  undefined2 uVar3;
  
  DAT_1080_298e = param_3;
  DAT_1080_2990 = param_4;
  uVar3 = 0x4532;
  pcVar2 = (char *)s_Lucky_Chip_1080_1004 + 4;
  uVar1 = MAKEPROCINSTANCE(unaff_CS,param_2,0x4014);
  DIALOGBOX(0x11d0,uVar1,pcVar2,param_1,uVar3);
  FREEPROCINSTANCE(0x11d0,(int)uVar1);
  return;
}



/* ============================================================
   1008:40c0  FUN_1008_40c0
   ============================================================ */

void __cdecl16far FUN_1008_40c0(int param_1,undefined2 param_2,undefined4 param_3)

{
  undefined2 *puVar1;
  byte *pbVar2;
  undefined2 *puVar3;
  uint uVar4;
  int iVar5;
  undefined2 in_DX;
  int iVar6;
  undefined2 *puVar7;
  undefined2 *puVar8;
  byte *pbVar9;
  undefined2 unaff_SS;
  byte local_42 [12];
  undefined2 local_36 [3];
  undefined2 local_30 [7];
  undefined2 local_22 [2];
  undefined2 local_1e [9];
  int local_c;
  undefined2 local_a;
  int local_8;
  int local_6;
  undefined2 local_4;
  
  uVar4 = FUN_1000_1710(param_1,param_2);
  local_c = FUN_1000_1646((uVar4 >> 1) + 2);
  iVar6 = 0;
  local_a = in_DX;
  while( true ) {
    iVar5 = FUN_1000_1710(param_1,param_2);
    if (iVar5 <= iVar6) break;
    *(byte *)(iVar6 / 2 + local_c) =
         *(char *)(param_1 + iVar6) << 4 ^ ((char *)(param_1 + iVar6))[1] & 0xfU;
    iVar6 = iVar6 + 2;
  }
  pbVar9 = local_42;
  puVar7 = (undefined2 *)0x4540;
  for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
    pbVar2 = pbVar9;
    pbVar9 = pbVar9 + 2;
    puVar1 = puVar7;
    puVar7 = puVar7 + 1;
    *(undefined2 *)pbVar2 = *puVar1;
  }
  puVar7 = local_36;
  for (iVar6 = 3; iVar6 != 0; iVar6 = iVar6 + -1) {
    puVar1 = puVar7;
    puVar7 = puVar7 + 1;
    *puVar1 = 0;
  }
  puVar7 = local_30;
  puVar8 = (undefined2 *)0x454c;
  for (iVar6 = 7; iVar6 != 0; iVar6 = iVar6 + -1) {
    puVar3 = puVar7;
    puVar7 = puVar7 + 1;
    puVar1 = puVar8;
    puVar8 = puVar8 + 1;
    *puVar3 = *puVar1;
  }
  puVar7 = local_22;
  for (iVar6 = 2; iVar6 != 0; iVar6 = iVar6 + -1) {
    puVar1 = puVar7;
    puVar7 = puVar7 + 1;
    *puVar1 = 0;
  }
  puVar7 = local_1e;
  puVar8 = (undefined2 *)0x455a;
  for (iVar6 = 9; iVar6 != 0; iVar6 = iVar6 + -1) {
    puVar3 = puVar7;
    puVar7 = puVar7 + 1;
    puVar1 = puVar8;
    puVar8 = puVar8 + 1;
    *puVar3 = *puVar1;
  }
  uVar4 = FUN_1000_1710(param_1,param_2);
  FUN_1000_1e38((int)param_3,param_3._2_2_,local_c,local_a,uVar4 >> 1);
  uVar4 = FUN_1000_1710(param_1,param_2);
  *(undefined1 *)((uVar4 >> 1) + (int)param_3) = 0;
  local_8 = 0;
  iVar6 = 0;
  while (iVar6 < 3) {
    local_6 = FUN_1000_1710(local_42 + iVar6 * 0x12);
    local_4 = 0;
    iVar5 = 0;
    local_8 = iVar6;
    while( true ) {
      uVar4 = FUN_1000_1710(param_1,param_2);
      if ((int)(uVar4 >> 1) <= iVar5) break;
      *(byte *)((int)param_3 + iVar5) =
           *(byte *)((int)param_3 + iVar5) ^ local_42[iVar5 % local_6 + local_8 * 0x12];
      iVar5 = iVar5 + 1;
    }
    iVar6 = local_8 + 1;
  }
  FUN_1000_1634(local_c,local_a);
  return;
}



/* ============================================================
   1008:4222  FUN_1008_4222
   ============================================================ */

void __cdecl16far FUN_1008_4222(undefined2 param_1,int param_2)

{
  undefined2 uVar1;
  int iVar2;
  char *pcVar3;
  int extraout_DX;
  int iVar4;
  undefined2 unaff_CS;
  undefined2 uVar5;
  undefined2 unaff_SS;
  undefined4 uVar6;
  undefined2 uVar7;
  char *pcVar8;
  undefined1 local_29e [260];
  undefined1 local_19a [260];
  undefined1 local_96 [136];
  char local_e [2];
  char *local_c;
  undefined2 local_a;
  undefined2 local_8;
  char *local_6;
  int iStack_4;
  
  if ((0 < param_2) && (param_2 < 3)) {
    GETMODULEFILENAME(unaff_CS,0x104,local_19a,unaff_SS);
    iVar4 = extraout_DX;
    iVar2 = FUN_1000_1e04(local_19a);
    _local_6 = CONCAT22(iVar4,iVar2);
    if (iVar4 != 0 || iVar2 != 0) {
      *(undefined1 *)(iVar2 + 1) = 0;
      FUN_1000_16aa(local_29e);
      FUN_1000_1656(local_29e);
      uVar6 = OPENFILE(0x1000,0x4000,local_96,unaff_SS,local_29e);
      local_a = (undefined2)((ulong)uVar6 >> 0x10);
      if ((int)uVar6 == -1) {
        pcVar3 = (char *)FUN_1000_1646(4000);
        local_e[0] = '\r';
        local_e[1] = 10;
        local_8 = OPENFILE(0x1000,0x1001,local_96,unaff_SS,local_29e);
        if (param_2 == 1) {
          FUN_1000_16aa(pcVar3,local_a,0x20e,0x1080);
          FUN_1000_1656(pcVar3,local_a,0x64c,0x1080);
        }
        else {
          FUN_1000_16aa(pcVar3,local_a,0xb82,0x1080);
        }
        uVar1 = local_8;
        iStack_4 = local_a;
        uVar5 = 0x1000;
        local_c = pcVar3;
        local_6 = pcVar3;
        for (; *pcVar3 != '\0'; pcVar3 = pcVar3 + 1) {
          if (*pcVar3 == '\n') {
            uVar7 = 2;
            pcVar8 = local_e;
            iVar4 = unaff_SS;
          }
          else {
            uVar7 = 1;
            pcVar8 = pcVar3;
            iVar4 = iStack_4;
          }
          _LWRITE(uVar5,uVar7,pcVar8,iVar4);
          uVar5 = 0x11d0;
        }
        uVar7 = uVar1;
        _LWRITE(uVar5,1,local_e + 1,unaff_SS);
        _LCLOSE(0x11d0,uVar1,uVar7);
        FUN_1000_1634(local_c,local_a);
      }
    }
  }
  return;
}



/* ============================================================
   1008:4584  ArsenalWindowProcedure
   ============================================================ */

void __stdcall16far
ArsenalWindowProcedure
          (undefined2 param_1,undefined2 param_2,undefined2 param_3,int param_4,undefined2 param_5)

{
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  
  if (param_4 == 1) {
    FUN_1008_47f4(*(undefined2 *)&DAT_1080_0d32,*(undefined2 *)&DAT_1080_0d34,param_1,param_2,
                  param_5);
  }
  else if (param_4 == 0xf) {
    FUN_1008_481e(*(undefined2 *)&DAT_1080_0d32,*(undefined2 *)&DAT_1080_0d34);
  }
  else if (param_4 == 0x111) {
    FUN_1008_47dc(*(undefined2 *)&DAT_1080_0d32,*(undefined2 *)&DAT_1080_0d34,param_1,param_2,
                  param_3);
  }
  else if (param_4 == 0x201) {
    FUN_1008_5770(*(undefined2 *)&DAT_1080_0d32,*(undefined2 *)&DAT_1080_0d34,param_1,param_2);
  }
  else if (param_4 == 0x202) {
    FUN_1008_5740(*(undefined2 *)&DAT_1080_0d32,*(undefined2 *)&DAT_1080_0d34,param_1,param_2);
  }
  else {
    DEFWINDOWPROC(unaff_CS,param_1,param_2,param_3,param_4);
  }
  return;
}



/* ============================================================
   1008:462a  FUN_1008_462a
   ============================================================ */

undefined2 * __stdcall16far
FUN_1008_462a(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  
  param_1[0x34] = 0;
  *param_1 = 0;
  param_1[1] = param_5;
  param_1[2] = param_6;
  param_1[3] = param_3;
  param_1[4] = param_4;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  for (iVar1 = 0; iVar1 < 0x14; iVar1 = iVar1 + 1) {
    param_1[iVar1 * 2 + 0xd] = 0;
    param_1[iVar1 * 2 + 0xc] = 0;
  }
  if (DAT_1080_0d34 != 0 || DAT_1080_0d32 != (undefined2 *)0x0) {
    FUN_1008_2d02(param_1[1],param_1[2],0,0x5b3e,(char *)s_Lucky_Chip_1080_1004 + 4);
  }
  DAT_1080_0d32 = param_1;
  DAT_1080_0d34 = param_2;
  FUN_1008_4774(param_1,param_2);
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1008:46ce  FUN_1008_46ce
   ============================================================ */

void __stdcall16far FUN_1008_46ce(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  
  for (iVar2 = 0; iVar2 < 0x14; iVar2 = iVar2 + 1) {
    iVar1 = iVar2 * 4 + param_1;
    if (*(int *)(iVar1 + 0x1a) != 0 || *(int *)(iVar1 + 0x18) != 0) {
      iVar1 = iVar2 * 4 + param_1;
      FUN_1000_1634(*(undefined2 *)(iVar1 + 0x18),*(undefined2 *)(iVar1 + 0x1a));
    }
  }
  return;
}



/* ============================================================
   1008:4712  FUN_1008_4712
   ============================================================ */

bool __stdcall16far
FUN_1008_4712(int *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  int *piVar2;
  undefined2 unaff_BP;
  undefined2 uVar3;
  undefined2 unaff_CS;
  
  uVar3 = (undefined2)((ulong)param_1 >> 0x10);
  piVar2 = (int *)param_1;
  iVar1 = CREATEWINDOW(unaff_CS,0,0,**(undefined2 **)(piVar2 + 1),1,param_6,param_2,param_3,param_4,
                       param_5,0,0x5000,0,0,0x5b6c,(char *)s_Lucky_Chip_1080_1004 + 4,unaff_BP);
  *param_1 = iVar1;
  if (iVar1 == 0) {
    FUN_1008_2d02(piVar2[1],piVar2[2],0,0x5b76,(char *)s_Lucky_Chip_1080_1004 + 4);
  }
  return iVar1 != 0;
}



/* ============================================================
   1008:4774  FUN_1008_4774
   ============================================================ */

undefined2 __stdcall16far FUN_1008_4774(undefined4 param_1)

{
  undefined2 unaff_CS;
  undefined2 uVar1;
  undefined2 local_1c;
  undefined2 local_1a;
  char *local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  undefined2 local_c;
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_6;
  char *local_4;
  
  local_1c = 0;
  local_1a = 0x4584;
  local_18 = (char *)s_Lucky_Chip_1080_1004 + 4;
  local_16 = 0;
  local_14 = 0;
  local_12 = *(undefined2 *)*(undefined4 *)((int)param_1 + 2);
  local_10 = 0;
  uVar1 = 0;
  local_e = LOADCURSOR(unaff_CS,0x7f00,0);
  local_c = GETSTOCKOBJECT(0x11d0,1,uVar1);
  local_8 = 0;
  local_a = 0;
  local_6 = 0x5b6c;
  local_4 = (char *)s_Lucky_Chip_1080_1004 + 4;
  REGISTERCLASS(0x11d0,&local_1c);
  return 1;
}



/* ============================================================
   1008:47dc  FUN_1008_47dc
   ============================================================ */

void __stdcall16far FUN_1008_47dc(void)

{
  SETFOCUS();
  return;
}



/* ============================================================
   1008:47f4  FUN_1008_47f4
   ============================================================ */

void __stdcall16far FUN_1008_47f4(int param_1,undefined2 param_2,undefined4 param_3)

{
  undefined2 uVar1;
  
  uVar1 = (undefined2)((ulong)param_3 >> 0x10);
  *(undefined2 *)(param_1 + 10) = *(undefined2 *)((int)param_3 + 0xc);
  *(undefined2 *)(param_1 + 0xc) = *(undefined2 *)((int)param_3 + 10);
  return;
}



/* ============================================================
   1008:481e  FUN_1008_481e
   ============================================================ */

void __stdcall16far FUN_1008_481e(undefined2 *param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 unaff_CS;
  char *pcVar2;
  undefined2 unaff_SS;
  undefined2 local_50 [2];
  undefined1 local_4c [28];
  undefined1 local_30 [8];
  undefined1 local_28 [8];
  undefined1 local_20 [8];
  undefined1 local_18 [8];
  undefined1 local_10 [8];
  undefined1 local_8 [6];
  
  BEGINPAINT(unaff_CS,local_50,unaff_SS,*param_1);
  FUN_1008_3b6c(local_8,unaff_SS,local_50[0]);
  SETRECT((char *)s_Lucky_Chip_1080_1004 + 4,0x2f,param_1[5],0,0,local_18);
  pcVar2 = (char *)0x11d0;
  iVar1 = INTERSECTRECT(0x11d0,local_4c,unaff_SS,local_18,unaff_SS,local_10);
  if (iVar1 != 0) {
    pcVar2 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_4e5e(param_1,param_2,local_50[0],local_8,unaff_SS);
  }
  SETRECT(pcVar2,0x1a5,param_1[5],0xc6,0,local_20);
  pcVar2 = (char *)0x11d0;
  iVar1 = INTERSECTRECT(0x11d0,local_4c,unaff_SS,local_20,unaff_SS,local_10);
  if (iVar1 != 0) {
    pcVar2 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_4976(param_1,param_2,local_50[0],local_8,unaff_SS);
  }
  SETRECT(pcVar2,0x55,param_1[5],0x2f,0,local_28);
  pcVar2 = (char *)0x11d0;
  iVar1 = INTERSECTRECT(0x11d0,local_4c,unaff_SS,local_28,unaff_SS,local_10);
  if (iVar1 != 0) {
    pcVar2 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_4f4e(param_1,param_2,local_50[0],local_8,unaff_SS);
  }
  SETRECT(pcVar2,0xc1,param_1[5],0x5f,0,local_30);
  pcVar2 = (char *)0x11d0;
  iVar1 = INTERSECTRECT(0x11d0,local_4c,unaff_SS,local_30,unaff_SS,local_10);
  if (iVar1 != 0) {
    pcVar2 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_520c(param_1,param_2,local_50[0]);
  }
  ENDPAINT(pcVar2,local_50,unaff_SS);
  FUN_1008_3ba6(local_8,unaff_SS);
  return;
}



/* ============================================================
   1008:4976  FUN_1008_4976
   ============================================================ */

void __stdcall16far
FUN_1008_4976(undefined4 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  undefined2 uVar1;
  undefined2 uVar2;
  int iVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  undefined2 unaff_SS;
  undefined1 *puVar9;
  undefined2 uVar10;
  undefined1 local_2e [20];
  undefined1 local_1a [6];
  undefined2 local_14;
  undefined2 local_12;
  undefined4 local_10;
  int local_c;
  undefined2 local_a;
  int local_8;
  undefined4 local_6;
  
  uVar1 = GETSTOCKOBJECT();
  SELECTOBJECT(0x11d0,uVar1);
  MOVETO(0x11d0,0x1a5,0);
  LINETO(0x11d0,0x1a5,*(int *)((int)param_1 + 10) + -1);
  LINETO(0x11d0,0xef,*(int *)((int)param_1 + 10) + -1);
  MOVETO(0x11d0,0x113,*(int *)((int)param_1 + 10) + -1);
  LINETO(0x11d0,0x113,0);
  MOVETO(0x11d0,0x137,0);
  LINETO(0x11d0,0x137,*(int *)((int)param_1 + 10) + -1);
  MOVETO(0x11d0,0x15c,0);
  LINETO(0x11d0,0x15c,*(int *)((int)param_1 + 10) + -1);
  MOVETO(0x11d0,0x180,0);
  LINETO(0x11d0,0x180,*(int *)((int)param_1 + 10) + -1);
  uVar1 = CREATEPEN(0x11d0,0x8080,0x80,1);
  local_6._0_2_ = SELECTOBJECT(0x11d0,uVar1);
  MOVETO(0x11d0,0x1a5,0);
  LINETO(0x11d0,0xef,0);
  LINETO(0x11d0,0xef,*(int *)((int)param_1 + 10) + -1);
  MOVETO(0x11d0,0x114,*(int *)((int)param_1 + 10) + -1);
  LINETO(0x11d0,0x114,0);
  MOVETO(0x11d0,0x138,0);
  LINETO(0x11d0,0x138,*(int *)((int)param_1 + 10) + -1);
  MOVETO(0x11d0,0x15d,0);
  uVar10 = param_2;
  LINETO(0x11d0,0x15d,*(int *)((int)param_1 + 10) + -1);
  uVar5 = param_2;
  MOVETO(0x11d0,0x181,0);
  uVar4 = param_2;
  LINETO(0x11d0,0x181,*(int *)((int)param_1 + 10) + -1);
  uVar1 = param_2;
  uVar2 = SELECTOBJECT(0x11d0,(int)local_6);
  DELETEOBJECT(0x11d0,uVar2,uVar1,uVar4,uVar5,uVar10);
  local_c = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,700,0,0,0
                      );
  if (local_c != 0) {
    local_8 = SELECTOBJECT(0x11d0,local_c);
    local_a = SETBKMODE(0x11d0,1);
    local_6 = SETTEXTCOLOR(0x11d0,0,0);
    TEXTOUT(0x11d0,7,0x5b98,(char *)s_Lucky_Chip_1080_1004 + 4,0xcd,2);
    SETTEXTCOLOR(0x11d0,0xffff,0xff);
    TEXTOUT(0x11d0,7,0x5b98,(char *)s_Lucky_Chip_1080_1004 + 4,0xcb,0);
    SETTEXTCOLOR(0x11d0,0x8080,0x40);
    uVar10 = param_2;
    TEXTOUT(0x11d0,7,0x5b98,(char *)s_Lucky_Chip_1080_1004 + 4,0xcc,1);
    uVar5 = param_2;
    SETTEXTCOLOR(0x11d0,local_6);
    uVar4 = param_2;
    SETBKMODE(0x11d0,local_a);
    uVar1 = param_2;
    uVar2 = SELECTOBJECT(0x11d0,local_8);
    DELETEOBJECT(0x11d0,uVar2,uVar1,uVar4,uVar5,uVar10);
    local_10._2_2_ = 0x9b;
    if (*(int *)((int)param_1 + 0x6a) == -1) {
      local_10._2_2_ = 0x99;
    }
    else if (*(int *)((int)param_1 + 0x6a) == 1) {
      local_10._2_2_ = 0x9a;
    }
    FUN_1008_3c2e(param_3,param_4,0xc6,*(int *)((int)param_1 + 10) + -0x19,local_10._2_2_);
    iVar3 = CREATEFONT((char *)s_Lucky_Chip_1080_1004 + 4,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,
                       0x31,0,0,5,0,0,0,0,400,0,0,0);
    if (iVar3 != 0) {
      local_6._0_2_ = iVar3;
      local_14 = SELECTOBJECT(0x11d0,iVar3);
      local_10 = SETTEXTCOLOR(0x11d0,0,0);
      uVar1 = 0x11d0;
      local_12 = SETBKMODE(0x11d0,1);
      local_6._2_2_ = 0;
      for (iVar3 = 0;
          (iVar3 < 5 &&
          (local_6._0_2_ = *(int *)((int)param_1 + 0x68) + iVar3,
          (int)local_6 < *(int *)((int)*(undefined4 *)((int)param_1 + 2) + 0x102)));
          iVar3 = iVar3 + 1) {
        uVar1 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 2) >> 0x10);
        iVar6 = (int)*(undefined4 *)((int)param_1 + 2) + (int)local_6 * 4;
        local_8 = *(int *)(iVar6 + 0x106);
        local_c = *(int *)(iVar6 + 0x108);
        local_a = *(undefined2 *)(local_8 * 0x22 + 0xd38);
        FUN_1000_16aa(local_2e);
        pcVar8 = (char *)s_Lucky_Chip_1080_1004 + 4;
        FUN_1008_3c2e(param_3,param_4,iVar3 * 0x24 + 0xf6,5,local_a);
        if (*(int *)((int)*(undefined4 *)((int)param_1 + 2) + 0x104) == (int)local_6) {
          FUN_1008_3c2e(param_3,param_4,iVar3 * 0x24 + 0x103,*(int *)((int)param_1 + 10) + -0x1a,
                        0x96);
          uVar1 = CREATEPEN((char *)s_Lucky_Chip_1080_1004 + 4,0,0x80,2);
          local_6._0_2_ = SELECTOBJECT(0x11d0,uVar1);
          iVar6 = (iVar3 * 0xb6) / 5 + 0xf0;
          MOVETO(0x11d0,iVar6,1);
          LINETO(0x11d0,iVar6,*(int *)((int)param_1 + 10) + -2);
          iVar7 = ((iVar3 + 1) * 0xb6) / 5 + 0xee;
          uVar10 = param_2;
          LINETO(0x11d0,iVar7,*(int *)((int)param_1 + 10) + -2);
          uVar5 = param_2;
          LINETO(0x11d0,iVar7,1);
          uVar4 = param_2;
          LINETO(0x11d0,iVar6,1);
          uVar1 = param_2;
          uVar2 = SELECTOBJECT(0x11d0,(int)local_6);
          pcVar8 = (char *)0x11d0;
          DELETEOBJECT(0x11d0,uVar2,uVar1,uVar4,uVar5,uVar10);
        }
        SETTEXTCOLOR(pcVar8,0,0);
        uVar5 = 0x24;
        iVar7 = local_8 * 0x22;
        iVar6 = (-(uint)(*(int *)(iVar7 + 0xd36) != 1 && *(int *)(iVar7 + 0xd36) != 0) & 0xfff9) +
                iVar3 * 0x24 + 0xf8;
        puVar9 = local_2e;
        uVar1 = unaff_SS;
        uVar4 = FUN_1000_1710(puVar9);
        TEXTOUT(0x1000,uVar4,puVar9,uVar1,iVar6,uVar5);
        SETTEXTCOLOR(0x11d0,0,0xff);
        uVar1 = 0x1000;
        FUN_1000_1822(0x11d0,local_c,local_1a);
        if ((*(int *)(iVar7 + 0xd36) != 0) && (*(int *)(local_8 * 0x22 + 0xd36) != 1)) {
          uVar10 = 0x24;
          iVar6 = iVar3 * 0x24 + 0xff;
          puVar9 = local_1a;
          uVar4 = unaff_SS;
          uVar5 = FUN_1000_1710(puVar9);
          uVar1 = 0x11d0;
          TEXTOUT(0x1000,uVar5,puVar9,uVar4,iVar6,uVar10);
        }
      }
      uVar4 = param_2;
      SETTEXTCOLOR(uVar1,local_10);
      uVar1 = param_2;
      SETBKMODE(0x11d0,local_12);
      uVar5 = SELECTOBJECT(0x11d0,local_14);
      DELETEOBJECT(0x11d0,uVar5,param_2,uVar1,uVar4);
    }
  }
  return;
}



/* ============================================================
   1008:4e5e  FUN_1008_4e5e
   ============================================================ */

void __stdcall16far FUN_1008_4e5e(undefined4 param_1,undefined2 param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined1 local_26 [10];
  int local_1c;
  int local_6;
  undefined2 local_4;
  
  uVar7 = param_2;
  SETBKMODE(unaff_CS,1);
  uVar6 = param_2;
  SETTEXTCOLOR(0x11d0,0x80,0);
  uVar3 = (undefined2)((ulong)param_1 >> 0x10);
  uVar1 = *(undefined4 *)((int)param_1 + 2);
  uVar1 = *(undefined4 *)((int)uVar1 + 0xc);
  uVar1 = *(undefined4 *)((int)uVar1 + 10);
  uVar5 = param_2;
  local_4 = SELECTOBJECT(0x11d0,*(undefined2 *)((int)uVar1 + 10));
  uVar4 = param_2;
  GETTEXTMETRICS(0x11d0,local_26,unaff_SS);
  iVar2 = local_1c;
  FUN_1008_3d50(param_2,local_1c,0,0x114,0x1080,uVar4,uVar5,uVar6,uVar7);
  FUN_1008_3d50(param_2,iVar2 * 2 + 1,0xf,0x116,0x1080);
  uVar1 = *(undefined4 *)((int)param_1 + 2);
  uVar1 = *(undefined4 *)((int)uVar1 + 0xc);
  uVar1 = *(undefined4 *)((int)uVar1 + 10);
  uVar5 = param_2;
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)((int)uVar1 + 0xe));
  uVar4 = param_2;
  GETTEXTMETRICS(0x11d0,local_26,unaff_SS);
  local_6 = local_1c;
  FUN_1008_3d50(param_2,iVar2 * 2,7,0x108,0x1080,uVar4,uVar5);
  FUN_1008_3d50(param_2,iVar2 * 3 + 1,0x16,0x10e,0x1080);
  TEXTOUT((char *)s_Lucky_Chip_1080_1004 + 4,1,0x5ba0,(char *)s_Lucky_Chip_1080_1004 + 4,0x16,
          (iVar2 + 1) * 3 + local_6 * 4);
  SELECTOBJECT(0x11d0,local_4);
  return;
}



/* ============================================================
   1008:4f4e  FUN_1008_4f4e
   ============================================================ */

void __stdcall16far
FUN_1008_4f4e(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  int iVar4;
  char *pcVar5;
  undefined2 unaff_SS;
  undefined1 *puVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined1 local_68 [50];
  undefined1 local_36 [10];
  int local_2c;
  undefined1 local_16 [8];
  undefined2 local_e;
  int local_c;
  int local_a;
  int local_8;
  int local_6;
  int local_4;
  
  uVar1 = GETSTOCKOBJECT();
  SELECTOBJECT(0x11d0,uVar1);
  MOVETO(0x11d0,0x55,0);
  LINETO(0x11d0,0x55,*(int *)(param_1 + 10) + -1);
  LINETO(0x11d0,0x2f,*(int *)(param_1 + 10) + -1);
  uVar1 = CREATEPEN(0x11d0,0x8080,0x80,1);
  local_4 = SELECTOBJECT(0x11d0,uVar1);
  uVar7 = param_3;
  MOVETO(0x11d0,0x2f,*(int *)(param_1 + 10) + -1);
  uVar3 = param_3;
  LINETO(0x11d0,0x2f,0);
  uVar8 = param_3;
  LINETO(0x11d0,0x55,0);
  uVar1 = param_3;
  uVar2 = SELECTOBJECT(0x11d0,local_4);
  DELETEOBJECT(0x11d0,uVar2,uVar1,uVar8,uVar3,uVar7);
  local_a = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,700,0,0,0
                      );
  if (local_a != 0) {
    local_e = SELECTOBJECT(0x11d0,local_a);
    GETTEXTMETRICS(0x11d0,local_36,unaff_SS);
    uVar1 = (undefined2)((ulong)*(undefined4 *)(param_1 + 2) >> 0x10);
    iVar4 = (int)*(undefined4 *)(param_1 + 2);
    FUN_1000_183e(0x11d0,*(undefined2 *)(iVar4 + 0xf6),*(undefined2 *)(iVar4 + 0xf8),local_68);
    local_c = FUN_1000_1710(local_68);
    uVar1 = param_3;
    SETBKMODE(0x1000,1);
    local_8 = 0;
    iVar4 = (*(int *)(param_1 + 10) - local_2c) + -6;
    local_4 = iVar4;
    local_6 = local_c;
    while (local_6 = local_6 + -1, -1 < local_6) {
      if ((local_8 % 3 == 0) && (local_8 != 0)) {
        SETTEXTCOLOR(0x11d0,0,0);
        TEXTOUT(0x11d0,1,0x5ba2,(char *)s_Lucky_Chip_1080_1004 + 4,0x30,iVar4 + 1);
        SETTEXTCOLOR(0x11d0,0xffff,0);
        TEXTOUT(0x11d0,1,0x5ba2,(char *)s_Lucky_Chip_1080_1004 + 4,0x2f,iVar4);
        iVar4 = iVar4 - local_2c;
      }
      SETTEXTCOLOR(0x11d0,0,0);
      puVar6 = local_68 + local_6;
      TEXTOUT(0x11d0,1,puVar6,unaff_SS,0x30,iVar4 + 1);
      SETTEXTCOLOR(0x11d0,0xffff,0);
      TEXTOUT(0x11d0,1,puVar6,unaff_SS,0x2f,iVar4);
      iVar4 = iVar4 - local_2c;
      local_8 = local_8 + 1;
    }
    uVar8 = param_3;
    uVar3 = SELECTOBJECT(0x11d0,local_e);
    pcVar5 = (char *)0x11d0;
    DELETEOBJECT(0x11d0,uVar3,uVar8,uVar1);
    local_4 = 0;
    for (iVar4 = 0; (iVar4 < *(int *)((int)*(undefined4 *)(param_1 + 2) + 0xf0) && (iVar4 < 7));
        iVar4 = iVar4 + 1) {
      pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
      FUN_1008_3c2e(param_4,param_5,0x48,iVar4 * 0xd + 2,0x154);
    }
    if ((7 < *(int *)((int)*(undefined4 *)(param_1 + 2) + 0xf0)) &&
       (local_a = CREATEFONT(pcVar5,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,700
                             ,0,0,0), local_a != 0)) {
      local_e = SELECTOBJECT(0x11d0,local_a);
      FUN_1000_1822(0x11d0,*(undefined2 *)((int)*(undefined4 *)(param_1 + 2) + 0xf0),local_16);
      uVar8 = param_3;
      SETTEXTCOLOR(0x1000,0,0xff);
      uVar2 = 0x5f;
      uVar7 = 0x48;
      puVar6 = local_16;
      uVar1 = param_3;
      uVar3 = FUN_1000_1710(puVar6);
      TEXTOUT(0x1000,uVar3,puVar6,unaff_SS,uVar7,uVar2);
      uVar3 = SELECTOBJECT(0x11d0,local_e);
      DELETEOBJECT(0x11d0,uVar3,param_3,uVar1,uVar8);
    }
  }
  return;
}



/* ============================================================
   1008:520c  FUN_1008_520c
   ============================================================ */

void __stdcall16far FUN_1008_520c(undefined4 param_1,undefined2 param_2)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  int iVar4;
  undefined2 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined2 uVar9;
  undefined2 unaff_SS;
  int *piVar10;
  undefined2 uVar11;
  int local_16;
  int local_14;
  undefined2 local_12;
  undefined2 uStack_10;
  int local_e;
  undefined4 local_c;
  undefined2 local_8;
  undefined2 local_6;
  int local_4;
  
  uVar1 = GETSTOCKOBJECT();
  SELECTOBJECT(0x11d0,uVar1);
  MOVETO(0x11d0,0xc1,0);
  uVar9 = (undefined2)((ulong)param_1 >> 0x10);
  iVar6 = (int)param_1;
  LINETO(0x11d0,0xc1,*(int *)(iVar6 + 10) + -1);
  LINETO(0x11d0,0x5f,*(int *)(iVar6 + 10) + -1);
  uVar1 = CREATEPEN(0x11d0,0x8080,0x80,1);
  uVar2 = SELECTOBJECT(0x11d0,uVar1);
  uVar11 = param_2;
  MOVETO(0x11d0,0xc1,0);
  uVar5 = param_2;
  LINETO(0x11d0,0x5f,0);
  uVar3 = param_2;
  LINETO(0x11d0,0x5f,*(int *)(iVar6 + 10) + -1);
  uVar1 = param_2;
  uVar2 = SELECTOBJECT(0x11d0,uVar2);
  DELETEOBJECT(0x11d0,uVar2,uVar1,uVar3,uVar5,uVar11);
  if (*(int *)(iVar6 + 0x10) < *(int *)(iVar6 + 0xe)) {
    iVar8 = *(int *)(iVar6 + 0x10) * 4;
    _local_12 = CONCAT22(*(undefined2 *)(iVar6 + iVar8 + 0x1a),*(undefined2 *)(iVar6 + iVar8 + 0x18)
                        );
    local_4 = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,400,0,0
                         ,0);
    if (local_4 != 0) {
      uVar1 = SELECTOBJECT(0x11d0,local_4);
      if (*(int *)(iVar6 + 0x10) < *(int *)(iVar6 + 0x12)) {
        uVar3 = 0x6060;
        uVar5 = 0x60;
      }
      else {
        uVar3 = 0;
        uVar5 = 0x80;
      }
      uVar11 = param_2;
      local_c = SETTEXTCOLOR(0x11d0,uVar3,uVar5);
      uVar3 = param_2;
      uVar5 = SETBKMODE(0x11d0,1);
      local_6 = 8;
      local_8 = 0xe;
      local_14 = 0x62;
      local_e = 0;
      do {
        local_16 = 3;
        iVar8 = local_e;
        while( true ) {
          while( true ) {
            uVar2 = 0x1000;
            iVar4 = FUN_1000_1710(local_12,uStack_10,uVar3,uVar11);
            if ((iVar4 <= iVar8) || (0xb3 < local_14)) {
              if (*(int *)(iVar6 + 0x10) < *(int *)(iVar6 + 0xe) + -1) {
                SETTEXTCOLOR(0x1000,0x6060,0x60);
                local_14 = DAT_1008_5ba4;
                _local_12 = CONCAT22(DAT_1008_5ba8,DAT_1008_5ba6);
                local_e = CONCAT11(local_e._1_1_,DAT_1008_5baa);
                uVar2 = 0x30;
                uVar11 = 0xb2;
                piVar10 = &local_14;
                uVar3 = FUN_1000_1710(piVar10);
                TEXTOUT(0x1000,uVar3,piVar10,unaff_SS,uVar11,uVar2);
                if (*(int *)(iVar6 + 0x10) < *(int *)(iVar6 + 0x12)) {
                  uVar3 = 0;
                }
                else {
                  uVar3 = 0xff;
                }
                SETTEXTCOLOR(0x11d0,0,uVar3);
                uVar2 = 0x11d0;
                TEXTOUT(0x11d0,1,0x5bac,(char *)s_Lucky_Chip_1080_1004 + 4,0xb3,0x37);
              }
              uVar11 = param_2;
              SETTEXTCOLOR(uVar2,local_c);
              uVar3 = param_2;
              SETBKMODE(0x11d0,uVar5);
              uVar1 = SELECTOBJECT(0x11d0,uVar1);
              DELETEOBJECT(0x11d0,uVar1,param_2,uVar3,uVar11);
              return;
            }
            uVar2 = (undefined2)((ulong)_local_12 >> 0x10);
            iVar4 = (int)_local_12;
            if (*(char *)(iVar4 + iVar8) != '\n') break;
            iVar8 = iVar8 + 1;
            local_16 = 3;
            local_14 = local_14 + 0xe;
          }
          iVar7 = 0;
          local_4 = 0;
          while (((*(char *)(iVar4 + iVar7 + iVar8) != ' ' &&
                  (*(char *)(iVar4 + iVar7 + iVar8) != '\n')) &&
                 (*(char *)(iVar4 + iVar7 + iVar8) != '\0'))) {
            local_4 = local_4 + 1;
            iVar7 = iVar7 + 1;
          }
          local_e = iVar8;
          if (*(int *)(iVar6 + 10) <= local_4 * 8 + local_16) break;
          if (local_16 == 3) {
            for (; local_e = iVar8, *(char *)(iVar4 + iVar8) == ' '; iVar8 = iVar8 + 1) {
            }
          }
          TEXTOUT(0x1000,1,iVar4 + local_e,uVar2,local_14,local_16);
          local_16 = local_16 + 8;
          if (*(int *)(iVar6 + 10) + -8 < local_16) {
            local_16 = 3;
            local_14 = local_14 + 0xe;
          }
          iVar8 = local_e + 1;
          local_e = iVar8;
        }
        local_14 = local_14 + 0xe;
      } while( true );
    }
  }
  return;
}



/* ============================================================
   1008:54da  FUN_1008_54da
   ============================================================ */

void __stdcall16far FUN_1008_54da(int param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 *puVar2;
  int iVar3;
  
  *(undefined2 *)(param_1 + 0xe) = 0;
  *(undefined2 *)(param_1 + 0x10) = 0;
  *(undefined2 *)(param_1 + 0x12) = 0;
  *(undefined2 *)(param_1 + 0x14) = 0;
  *(undefined2 *)(param_1 + 0x16) = 0;
  for (iVar3 = 0; iVar3 < 0x14; iVar3 = iVar3 + 1) {
    iVar1 = iVar3 * 4 + param_1;
    if (*(int *)(iVar1 + 0x1a) != 0 || *(int *)(iVar1 + 0x18) != 0) {
      puVar2 = (undefined2 *)(param_1 + 0x18 + iVar3 * 4);
      FUN_1000_1634(*puVar2,puVar2[1]);
      iVar1 = param_1 + iVar3 * 4;
      *(undefined2 *)(iVar1 + 0x1a) = 0;
      *(undefined2 *)(iVar1 + 0x18) = 0;
    }
  }
  return;
}



/* ============================================================
   1008:554e  FUN_1008_554e
   ============================================================ */

void __stdcall16far FUN_1008_554e(int param_1,undefined2 param_2)

{
  undefined2 uVar1;
  
  if (*(int *)(param_1 + 0x16) != 0) {
    uVar1 = *(undefined2 *)(param_1 + 0x14);
    *(undefined2 *)(param_1 + 0x10) = uVar1;
    *(undefined2 *)(param_1 + 0x12) = uVar1;
    FUN_1008_5b04(param_1,param_2);
  }
  *(undefined2 *)(param_1 + 0x16) = 0;
  *(undefined2 *)(param_1 + 0x14) = *(undefined2 *)(param_1 + 0xe);
  return;
}



/* ============================================================
   1008:5588  FUN_1008_5588
   ============================================================ */

void __stdcall16far FUN_1008_5588(undefined4 param_1)

{
  int iVar1;
  undefined2 uVar2;
  
  uVar2 = (undefined2)((ulong)param_1 >> 0x10);
  iVar1 = (int)param_1;
  if (*(int *)(iVar1 + 0xe) != *(int *)(iVar1 + 0x10)) {
    *(int *)(iVar1 + 0x10) = *(int *)(iVar1 + 0x10) + 1;
    if (*(int *)(iVar1 + 0x12) < *(int *)(iVar1 + 0x10)) {
      *(undefined2 *)(iVar1 + 0x12) = *(undefined2 *)(iVar1 + 0x10);
    }
    FUN_1008_5b04(iVar1,uVar2);
  }
  return;
}



/* ============================================================
   1008:55be  FUN_1008_55be
   ============================================================ */

void __stdcall16far FUN_1008_55be(int param_1,undefined2 param_2)

{
  if (0 < *(int *)(param_1 + 0x10)) {
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + -1;
    FUN_1008_5b04(param_1,param_2);
  }
  return;
}



/* ============================================================
   1008:55e0  FUN_1008_55e0
   ============================================================ */

void __stdcall16far FUN_1008_55e0(undefined4 param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 in_DX;
  undefined2 extraout_DX;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  if (*(int *)(iVar4 + 0xe) < 0x15) {
    if (*(int *)(iVar4 + 0xe) == 0x14) {
      FUN_1008_56e6(iVar4,uVar5);
      if (0 < *(int *)(iVar4 + 0x10)) {
        *(int *)(iVar4 + 0x10) = *(int *)(iVar4 + 0x10) + -1;
      }
      if (0 < *(int *)(iVar4 + 0x12)) {
        *(int *)(iVar4 + 0x12) = *(int *)(iVar4 + 0x12) + -1;
      }
      if (0 < *(int *)(iVar4 + 0x14)) {
        *(int *)(iVar4 + 0x14) = *(int *)(iVar4 + 0x14) + -1;
      }
      uVar2 = extraout_DX;
      uVar1 = FUN_1000_1646(100);
      *(undefined2 *)(iVar4 + 100) = uVar1;
      *(undefined2 *)(iVar4 + 0x66) = uVar2;
      FUN_1000_1e96(*(undefined2 *)(iVar4 + 100),uVar2,0,100);
      FUN_1000_1762(*(undefined2 *)(iVar4 + 100),*(undefined2 *)(iVar4 + 0x66),param_2,param_3,99);
    }
    else {
      uVar2 = FUN_1000_1646(100);
      iVar3 = *(int *)(iVar4 + 0xe) * 4;
      *(undefined2 *)(iVar3 + iVar4 + 0x18) = uVar2;
      *(undefined2 *)(iVar3 + iVar4 + 0x1a) = in_DX;
      iVar3 = *(int *)(iVar4 + 0xe) * 4;
      FUN_1000_1e96(*(undefined2 *)(iVar3 + iVar4 + 0x18),*(undefined2 *)(iVar3 + iVar4 + 0x1a),0,
                    100);
      iVar3 = *(int *)(iVar4 + 0xe) * 4;
      FUN_1000_1762(*(undefined2 *)(iVar3 + iVar4 + 0x18),*(undefined2 *)(iVar3 + iVar4 + 0x1a),
                    param_2,param_3,99);
      *(int *)(iVar4 + 0xe) = *(int *)(iVar4 + 0xe) + 1;
    }
    *(undefined2 *)(iVar4 + 0x16) = 1;
  }
  else {
    FUN_1008_2d02(*(undefined2 *)(iVar4 + 2),*(undefined2 *)(iVar4 + 4),0,0x5bae,
                  (char *)s_Lucky_Chip_1080_1004 + 4);
  }
  return;
}



/* ============================================================
   1008:56e6  FUN_1008_56e6
   ============================================================ */

void __stdcall16far FUN_1008_56e6(int param_1,undefined2 param_2)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  
  if (*(int *)(param_1 + 0x1a) != 0 || *(int *)(param_1 + 0x18) != 0) {
    FUN_1000_1634(*(undefined2 *)(param_1 + 0x18),*(undefined2 *)(param_1 + 0x1a));
  }
  iVar2 = 0;
  do {
    iVar3 = iVar2 * 4 + param_1;
    uVar1 = *(undefined2 *)(iVar3 + 0x1e);
    *(undefined2 *)(iVar3 + 0x18) = *(undefined2 *)(iVar3 + 0x1c);
    *(undefined2 *)(iVar3 + 0x1a) = uVar1;
    iVar2 = iVar2 + 1;
  } while (iVar2 < 0x13);
  *(undefined2 *)(param_1 + 0x66) = 0;
  *(undefined2 *)(param_1 + 100) = 0;
  return;
}



/* ============================================================
   1008:5740  FUN_1008_5740
   ============================================================ */

void __stdcall16far FUN_1008_5740(int param_1,undefined2 param_2)

{
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 2) + 0xdc);
  if (*(int *)((int)uVar1 + 0xc) == 0) {
    *(undefined2 *)(param_1 + 0x6a) = 0;
    FUN_1008_5a4c(param_1,param_2);
  }
  return;
}



/* ============================================================
   1008:5770  FUN_1008_5770
   ============================================================ */

void __stdcall16far FUN_1008_5770(undefined2 *param_1,undefined2 param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined2 uVar4;
  char *pcVar5;
  undefined2 unaff_SS;
  undefined2 uVar6;
  undefined1 local_16 [4];
  int local_12;
  undefined2 local_6;
  uint local_4;
  
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 1) + 0xdc);
  if (*(int *)((int)uVar1 + 0xc) != 0) {
    return;
  }
  local_4 = param_3;
  if ((((0xc6 < param_4) && (param_4 < 0xea)) && (param_1[5] + -0x1a < param_3)) &&
     (param_3 < param_1[5] + -1)) {
    if (param_4 < (param_1[5] - param_3) + 0xb9) {
      return;
    }
    if (param_4 < (param_3 - param_1[5]) + 0xd3) {
      return;
    }
    if ((param_3 - param_1[5]) + 0xf7 < param_4) {
      return;
    }
    if ((param_1[5] - param_3) + 0xdd < param_4) {
      return;
    }
    if (param_4 < 0xd9) {
      uVar4 = 0xffff;
    }
    else {
      uVar4 = 1;
    }
    param_1[0x35] = uVar4;
    FUN_1008_5a4c(param_1,param_2);
    uVar3 = (uint)(param_4 < 0xd9);
    FUN_1008_5a04(param_1,param_2,uVar3);
    SETCAPTURE((char *)s_Lucky_Chip_1080_1004 + 4,*param_1);
    uVar4 = *param_1;
    SETTIMER(0x11d0,0,0,500,0xc6);
    local_4 = uVar3;
    while( true ) {
      pcVar5 = (char *)0x11d0;
      iVar2 = PEEKMESSAGE(0x11d0,1,0x113,0x113,0,local_16,unaff_SS);
      if (iVar2 != 0) {
        if (local_12 == 0xc6) {
          KILLTIMER(0x11d0,0xc6);
          SETTIMER(0x11d0,0,0,100,199);
        }
        pcVar5 = (char *)0x11d0;
        if (local_12 == 199) {
          pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
          FUN_1008_5a04(param_1,param_2,uVar3 != 0);
        }
      }
      iVar2 = PEEKMESSAGE(pcVar5,1,0x202,0x202,0,local_16,unaff_SS);
      if (iVar2 != 0) break;
      iVar2 = PEEKMESSAGE(0x11d0,0,0xf,0xf,0,local_16,unaff_SS);
      if (iVar2 != 0) {
        GETMESSAGE(0x11d0,0xf,0xf,0,local_16,unaff_SS,uVar4);
        uVar4 = unaff_SS;
        DISPATCHMESSAGE(0x11d0,local_16);
      }
    }
    param_1[0x35] = 0;
    FUN_1008_5a4c(param_1,param_2);
    uVar4 = *param_1;
    KILLTIMER((char *)s_Lucky_Chip_1080_1004 + 4,0xc6);
    uVar6 = *param_1;
    KILLTIMER(0x11d0,199);
    RELEASECAPTURE(0x11d0,uVar6,uVar4);
    return;
  }
  if (param_4 < 0xf0) {
    return;
  }
  if (0x1a4 < param_4) {
    return;
  }
  local_4 = *(undefined2 *)((int)*(undefined4 *)(param_1 + 1) + 0x104);
  local_6 = 0x24;
  if ((param_4 < 0xf0) || (0x112 < param_4)) {
    if ((param_4 < 0x114) || (0x136 < param_4)) {
      if ((param_4 < 0x138) || (0x15a < param_4)) {
        if ((param_4 < 0x15c) || (0x17e < param_4)) {
          if ((param_4 < 0x180) || (0x1a4 < param_4)) goto LAB_1008_59d1;
          iVar2 = param_1[0x34] + 4;
        }
        else {
          iVar2 = param_1[0x34] + 3;
        }
      }
      else {
        iVar2 = param_1[0x34] + 2;
      }
    }
    else {
      iVar2 = param_1[0x34] + 1;
    }
  }
  else {
    iVar2 = param_1[0x34];
  }
  *(int *)((int)*(undefined4 *)(param_1 + 1) + 0x104) = iVar2;
LAB_1008_59d1:
  uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 1) >> 0x10);
  iVar2 = (int)*(undefined4 *)(param_1 + 1);
  if (*(int *)(iVar2 + 0x104) < *(int *)(iVar2 + 0x102)) {
    FUN_1008_5a8a(param_1,param_2);
  }
  else {
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 1) + 0x104) = local_4;
  }
  return;
}



/* ============================================================
   1008:5a04  FUN_1008_5a04
   ============================================================ */

void __stdcall16far FUN_1008_5a04(undefined4 param_1,int param_2)

{
  if (param_2 == 0) {
    if (*(int *)((int)param_1 + 0x68) + 5 < *(int *)((int)*(undefined4 *)((int)param_1 + 2) + 0x102)
       ) {
      *(int *)((int)param_1 + 0x68) = *(int *)((int)param_1 + 0x68) + 1;
    }
  }
  else if (0 < *(int *)((int)param_1 + 0x68)) {
    *(int *)((int)param_1 + 0x68) = *(int *)((int)param_1 + 0x68) + -1;
  }
  FUN_1008_5a8a((int)param_1,param_1._2_2_);
  return;
}



/* ============================================================
   1008:5a4c  FUN_1008_5a4c
   ============================================================ */

void __stdcall16far FUN_1008_5a4c(undefined2 *param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined1 local_a [8];
  
  SETRECT(unaff_CS,0xea,param_1[5] + -1,0xc6,0,local_a);
  INVALIDATERECT(0x11d0,1,local_a,unaff_SS,*param_1);
  return;
}



/* ============================================================
   1008:5a8a  FUN_1008_5a8a
   ============================================================ */

void __stdcall16far FUN_1008_5a8a(undefined2 *param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined1 local_a [8];
  
  SETRECT(unaff_CS,0x1a5,param_1[5] + -1,0xef,0,local_a);
  INVALIDATERECT(0x11d0,1,local_a,unaff_SS,*param_1);
  return;
}



/* ============================================================
   1008:5ac8  FUN_1008_5ac8
   ============================================================ */

void __stdcall16far FUN_1008_5ac8(undefined2 *param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined1 local_a [8];
  
  SETRECT(unaff_CS,0x55,param_1[5] + -1,0x2f,0,local_a);
  INVALIDATERECT(0x11d0,1,local_a,unaff_SS,*param_1);
  return;
}



/* ============================================================
   1008:5b04  FUN_1008_5b04
   ============================================================ */

void __stdcall16far FUN_1008_5b04(undefined2 *param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined1 local_a [8];
  
  SETRECT(unaff_CS,0xc1,param_1[5],0x5f,0,local_a);
  INVALIDATERECT(0x11d0,1,local_a,unaff_SS,*param_1);
  return;
}



/* ============================================================
   1008:5be4  BoardWindowProcedure
   ============================================================ */

void __stdcall16far
BoardWindowProcedure
          (undefined2 param_1,undefined2 param_2,undefined2 param_3,int param_4,undefined2 param_5)

{
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  
  if (param_4 == 1) {
    FUN_1008_5f46(*(undefined2 *)&DAT_1080_1022,*(undefined2 *)&DAT_1080_1024,param_5);
  }
  else if (param_4 == 2) {
    FUN_1008_6032(*(undefined2 *)&DAT_1080_1022,*(undefined2 *)&DAT_1080_1024);
  }
  else if (param_4 == 0xf) {
    FUN_1008_5e70(*(undefined2 *)&DAT_1080_1022,*(undefined2 *)&DAT_1080_1024);
  }
  else if (param_4 == 0x14) {
    FUN_1008_a5fa(0x1080);
  }
  else if (param_4 == 0x113) {
    FUN_1008_5f92(*(undefined2 *)&DAT_1080_1022,*(undefined2 *)&DAT_1080_1024,param_3);
  }
  else if (param_4 == 0x201) {
    FUN_1008_6048(*(undefined2 *)&DAT_1080_1022,*(undefined2 *)&DAT_1080_1024,param_1,param_2,
                  param_3);
  }
  else if (param_4 == 0x204) {
    FUN_1008_60f4(*(undefined2 *)&DAT_1080_1022,*(undefined2 *)&DAT_1080_1024,param_1,param_2,
                  param_3);
  }
  else {
    DEFWINDOWPROC(unaff_CS,param_1,param_2,param_3,param_4);
  }
  return;
}



/* ============================================================
   1008:5ca4  FUN_1008_5ca4
   ============================================================ */

undefined2 * __stdcall16far
FUN_1008_5ca4(undefined2 *param_1,undefined2 param_2,undefined2 param_3,int param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  undefined4 uVar2;
  
  param_1[4] = 0;
  *param_1 = param_5;
  param_1[1] = param_6;
  param_1[2] = param_3;
  param_1[3] = param_4;
  param_1[6] = 0;
  param_1[5] = 0;
  iVar1 = FUN_1000_1646(0x34);
  if (param_4 == 0 && iVar1 == 0) {
    param_1[6] = 0;
    param_1[5] = 0;
  }
  else {
    uVar2 = FUN_1010_7642(iVar1,param_4,param_1,param_2,*param_1,param_1[1]);
    param_1[5] = (int)uVar2;
    param_1[6] = (int)((ulong)uVar2 >> 0x10);
  }
  if (DAT_1080_1024 != 0 || DAT_1080_1022 != (undefined2 *)0x0) {
    FUN_1008_2d02(*param_1,param_1[1],0,0x681e,(char *)s_Lucky_Chip_1080_1004 + 4);
  }
  DAT_1080_1022 = param_1;
  DAT_1080_1024 = param_2;
  FUN_1008_5e0e(param_1,param_2);
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1008:5d56  FUN_1008_5d56
   ============================================================ */

void __stdcall16far FUN_1008_5d56(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  
  uVar4 = (undefined2)((ulong)param_1 >> 0x10);
  iVar3 = (int)param_1;
  if (*(int *)(iVar3 + 0xc) != 0 || *(int *)(iVar3 + 10) != 0) {
    iVar1 = *(int *)(iVar3 + 10);
    iVar2 = *(int *)(iVar3 + 0xc);
    if (iVar2 != 0 || iVar1 != 0) {
      FUN_1010_77c2(iVar1,iVar2);
      FUN_1000_1634(iVar1,iVar2);
    }
    *(undefined2 *)(iVar3 + 0xc) = 0;
    *(undefined2 *)(iVar3 + 10) = 0;
  }
  return;
}



/* ============================================================
   1008:5dac  FUN_1008_5dac
   ============================================================ */

bool __stdcall16far
FUN_1008_5dac(undefined4 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  undefined2 unaff_BP;
  undefined2 uVar2;
  undefined2 unaff_CS;
  
  iVar1 = CREATEWINDOW(unaff_CS,0,0,*(undefined2 *)*param_1,2,param_6,param_2,param_3,param_4,
                       param_5,0,0x5280,0,0,0x684a,(char *)s_Lucky_Chip_1080_1004 + 4,unaff_BP);
  uVar2 = (undefined2)((ulong)param_1 >> 0x10);
  *(int *)((undefined4 *)param_1 + 2) = iVar1;
  if (iVar1 == 0) {
    FUN_1008_2d02(*(undefined2 *)param_1,*(undefined2 *)((int)(undefined4 *)param_1 + 2),0,0x6854,
                  (char *)s_Lucky_Chip_1080_1004 + 4);
  }
  return iVar1 != 0;
}



/* ============================================================
   1008:5e0e  FUN_1008_5e0e
   ============================================================ */

undefined2 __stdcall16far FUN_1008_5e0e(undefined4 *param_1)

{
  undefined2 unaff_CS;
  undefined2 local_1c;
  undefined2 local_1a;
  char *local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  undefined2 local_c;
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_6;
  char *local_4;
  
  local_1c = 0;
  local_1a = 0x5be4;
  local_18 = (char *)s_Lucky_Chip_1080_1004 + 4;
  local_16 = 0;
  local_14 = 0;
  local_12 = *(undefined2 *)*param_1;
  local_10 = 0;
  local_e = LOADCURSOR(unaff_CS,0x7f00,0);
  local_c = 0;
  local_8 = 0;
  local_a = 0;
  local_6 = 0x684a;
  local_4 = (char *)s_Lucky_Chip_1080_1004 + 4;
  REGISTERCLASS(0x11d0,&local_1c);
  return 1;
}



/* ============================================================
   1008:5e70  FUN_1008_5e70
   ============================================================ */

void __stdcall16far FUN_1008_5e70(undefined4 *param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 local_28 [16];
  undefined1 local_8 [6];
  
  BEGINPAINT(unaff_CS,local_28,unaff_SS,*(undefined2 *)(param_1 + 2));
  FUN_1008_3b6c(local_8,unaff_SS,local_28[0]);
  if (*(int *)((int)*param_1 + 6) != 0) {
    FUN_1008_a5fa();
  }
  uVar2 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar1 = (int)*param_1;
  FUN_1008_9070(*(undefined2 *)(iVar1 + 0xdc),*(undefined2 *)(iVar1 + 0xde),local_8,unaff_SS);
  uVar2 = FUN_1008_2d50(*(undefined2 *)param_1,*(undefined2 *)((int)param_1 + 2));
  uVar3 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar1 = (int)*param_1;
  FUN_1008_3bd0(local_8,unaff_SS,*(undefined2 *)(iVar1 + 0xec),*(undefined2 *)(iVar1 + 0xea),uVar2);
  uVar2 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar1 = (int)*param_1;
  FUN_1008_8ea4(*(undefined2 *)(iVar1 + 0xdc),*(undefined2 *)(iVar1 + 0xde),local_8,unaff_SS);
  uVar2 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar1 = (int)*param_1;
  FUN_1008_8f32(*(undefined2 *)(iVar1 + 0xdc),*(undefined2 *)(iVar1 + 0xde),local_8,unaff_SS);
  uVar2 = *(undefined2 *)(param_1 + 2);
  ENDPAINT((char *)s_Lucky_Chip_1080_1004 + 4,local_28,unaff_SS);
  FUN_1008_a626(uVar2);
  FUN_1008_3ba6(local_8,unaff_SS);
  return;
}



/* ============================================================
   1008:5f46  FUN_1008_5f46
   ============================================================ */

void __stdcall16far FUN_1008_5f46(int param_1,undefined2 param_2,undefined2 param_3)

{
  FUN_1008_a5fa();
  SETTIMER((char *)s_Lucky_Chip_1080_1004 + 4,0,0,200,0xb);
  if (*(int *)(param_1 + 0xc) != 0 || *(int *)(param_1 + 10) != 0) {
    FUN_1010_7816(*(undefined2 *)(param_1 + 10),*(undefined2 *)(param_1 + 0xc),0x90,0xf0,0x120,0x9c,
                  param_3);
  }
  return;
}



/* ============================================================
   1008:5f92  FUN_1008_5f92
   ============================================================ */

void __stdcall16far FUN_1008_5f92(undefined4 *param_1,undefined2 param_2,int param_3)

{
  undefined2 *puVar1;
  undefined4 uVar2;
  int iVar3;
  undefined2 uVar4;
  char *pcVar5;
  
  if ((param_3 == 0xb) &&
     (uVar4 = (undefined2)((ulong)*param_1 >> 0x10), iVar3 = (int)*param_1,
     *(int *)(iVar3 + 0xde) != 0 || *(int *)(iVar3 + 0xdc) != 0)) {
    uVar4 = (undefined2)((ulong)*param_1 >> 0x10);
    iVar3 = (int)*param_1;
    pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_9680(*(undefined2 *)(iVar3 + 0xdc),*(undefined2 *)(iVar3 + 0xde));
    uVar4 = (undefined2)((ulong)*param_1 >> 0x10);
    iVar3 = (int)*param_1;
    if (*(int *)(iVar3 + 0xd4) != 0 || *(int *)(iVar3 + 0xd2) != 0) {
      uVar4 = (undefined2)((ulong)*param_1 >> 0x10);
      iVar3 = (int)*param_1;
      pcVar5 = (char *)0x1010;
      FUN_1010_afd8(*(undefined2 *)(iVar3 + 0xd2),*(undefined2 *)(iVar3 + 0xd4));
    }
    uVar2 = *(undefined4 *)((int)*param_1 + 0xdc);
    if (*(int *)((int)uVar2 + 0xc) != 0) {
      uVar4 = (undefined2)((ulong)*param_1 >> 0x10);
      iVar3 = (int)*param_1;
      puVar1 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(iVar3 + 0xdc) + 0x2c);
      (*(code *)*puVar1)(pcVar5,*(undefined2 *)(iVar3 + 0xdc),*(undefined2 *)(iVar3 + 0xde));
    }
    FUN_1010_808a(*(undefined2 *)((int)param_1 + 10),*(undefined2 *)(param_1 + 3));
  }
  return;
}



/* ============================================================
   1008:6032  FUN_1008_6032
   ============================================================ */

void __stdcall16far FUN_1008_6032(void)

{
  undefined2 unaff_CS;
  
  KILLTIMER(unaff_CS,0xb);
  return;
}



/* ============================================================
   1008:6048  FUN_1008_6048
   ============================================================ */

void __stdcall16far FUN_1008_6048(undefined4 *param_1,int param_2,int param_3,byte param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined2 uVar4;
  int iVar5;
  undefined2 uVar6;
  
  uVar2 = *(undefined4 *)((int)*param_1 + 0xdc);
  if (*(int *)((int)uVar2 + 0xc) == 0) {
    uVar6 = (undefined2)((ulong)*param_1 >> 0x10);
    iVar5 = (int)*param_1;
    iVar1 = *(int *)(iVar5 + 0xea);
    iVar5 = *(int *)(iVar5 + 0xec);
    param_2 = param_2 / 0xc;
    param_3 = param_3 / 0xc;
    iVar3 = FUN_1000_1f42(param_2 - iVar1);
    if ((iVar3 < 2) && (iVar3 = FUN_1000_1f42(param_3 - iVar5), iVar3 < 2)) {
      if (iVar1 < param_2) {
        uVar6 = 1;
      }
      else if (param_2 == iVar1) {
        uVar6 = 0;
      }
      else {
        uVar6 = 0xffff;
      }
      if (iVar5 < param_3) {
        uVar4 = 1;
      }
      else if (param_3 == iVar5) {
        uVar4 = 0;
      }
      else {
        uVar4 = 0xffff;
      }
      FUN_1008_1b22(*(undefined2 *)param_1,*(undefined2 *)((int)(undefined4 *)param_1 + 2),
                    param_4 & 4,uVar4,uVar6);
    }
  }
  return;
}



/* ============================================================
   1008:60f4  FUN_1008_60f4
   ============================================================ */

void __stdcall16far FUN_1008_60f4(undefined4 *param_1,int param_2,int param_3)

{
  int *piVar1;
  undefined2 *puVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  undefined2 uVar8;
  undefined2 unaff_SS;
  char *pcVar9;
  undefined1 local_2a [16];
  undefined1 local_1a [10];
  int local_10;
  int local_e;
  int local_c;
  int local_a;
  int local_8;
  int local_6;
  int local_4;
  
  uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
  if (*(int *)((int)uVar3 + 0xc) != 0) {
    return;
  }
  param_2 = param_2 / 0xc;
  param_3 = param_3 / 0xc;
  local_10 = param_2;
  local_e = param_3;
  if ((*(int *)((int)*param_1 + 0xea) == param_2) && (*(int *)((int)*param_1 + 0xec) == param_3)) {
    pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
    unaff_SS = 0x6876;
    goto LAB_1008_67da;
  }
  uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
  uVar8 = (undefined2)((ulong)uVar3 >> 0x10);
  iVar5 = (int)uVar3;
  local_6 = *(int *)(iVar5 + 0x2c);
  local_4 = *(int *)(iVar5 + 0x2e);
  local_c = 0;
  for (iVar5 = 0; uVar3 = *(undefined4 *)((int)*param_1 + 0xdc), iVar5 < *(int *)((int)uVar3 + 0x50)
      ; iVar5 = iVar5 + 1) {
    if ((*(int *)(iVar5 * 0xc + local_6 + 2) == param_2) &&
       (*(int *)(iVar5 * 0xc + local_6 + 4) == param_3)) {
      iVar6 = *(int *)(iVar5 * 0xc + local_6);
      if (iVar6 == 0) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x689a;
        goto LAB_1008_67da;
      }
      if (iVar6 == 1) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x68a6;
        goto LAB_1008_67da;
      }
      if (iVar6 == 2) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x68b0;
        goto LAB_1008_67da;
      }
      if (iVar6 == 3) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x68d4;
        goto LAB_1008_67da;
      }
      if (iVar6 == 4) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x68f6;
        goto LAB_1008_67da;
      }
    }
  }
  uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
  uVar8 = (undefined2)((ulong)uVar3 >> 0x10);
  iVar5 = (int)uVar3;
  local_8 = *(int *)(iVar5 + 0x32);
  local_6 = *(int *)(iVar5 + 0x34);
  for (iVar5 = 0; uVar3 = *(undefined4 *)((int)*param_1 + 0xdc), iVar5 < *(int *)((int)uVar3 + 0x52)
      ; iVar5 = iVar5 + 1) {
    if ((*(int *)(iVar5 * 10 + local_8 + 2) == param_2) &&
       (*(int *)(iVar5 * 10 + local_8 + 4) == param_3)) {
      iVar6 = *(int *)(iVar5 * 10 + local_8);
      if (iVar6 == 0) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x691e;
        goto LAB_1008_67da;
      }
      if (iVar6 == 1) {
        if ((*(int *)(iVar5 * 10 + local_8 + 6) == 0) && (*(int *)(iVar5 * 10 + local_8 + 8) == 0))
        {
          local_a = 1;
          local_4 = 0;
          for (iVar6 = 0; uVar3 = *(undefined4 *)((int)*param_1 + 0xdc),
              iVar6 < *(int *)((int)uVar3 + 0x52); iVar6 = iVar6 + 1) {
            if ((((iVar5 != iVar6) && (*(int *)(iVar6 * 10 + local_8 + 2) == param_2)) &&
                (*(int *)(iVar6 * 10 + local_8 + 4) == param_3)) &&
               ((*(int *)(iVar6 * 10 + local_8 + 6) == 0 &&
                (*(int *)(iVar6 * 10 + local_8 + 8) == 0)))) {
              local_a = local_a + 1;
            }
          }
          local_c = iVar5;
          FUN_1000_16aa(local_2a);
          pcVar9 = (char *)0xa;
          FUN_1000_1822(0x1000,local_a,local_1a);
          FUN_1000_1656(local_2a);
          FUN_1000_1656(local_2a);
        }
        else {
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          unaff_SS = 0x6932;
        }
        goto LAB_1008_67da;
      }
      if (iVar6 == 2) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x6938;
        goto LAB_1008_67da;
      }
      if (iVar6 == 3) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x6940;
        goto LAB_1008_67da;
      }
      if (iVar6 == 4) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x6972;
        goto LAB_1008_67da;
      }
    }
  }
  uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
  uVar8 = (undefined2)((ulong)uVar3 >> 0x10);
  iVar5 = (int)uVar3;
  uVar3 = *(undefined4 *)(iVar5 + 8);
  uVar3 = *(undefined4 *)((int)uVar3 + param_2 * 4);
  uVar4 = FUN_1008_916e(iVar5,uVar8,*(undefined2 *)((int)uVar3 + param_3 * 2));
  iVar5 = local_10;
  if ((uVar4 & 2) == 0) {
    uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
    uVar3 = *(undefined4 *)((int)uVar3 + 0x36);
    uVar8 = (undefined2)((ulong)uVar3 >> 0x10);
    iVar6 = (int)uVar3;
    local_4 = *(int *)(iVar6 + 4);
    piVar7 = (int *)*(undefined2 *)(iVar6 + 2);
    while (local_4 != 0 || piVar7 != (int *)0x0) {
      if ((*piVar7 == local_10) && (piVar7[1] == param_3)) {
        switch(piVar7[2]) {
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xf:
        case 0x10:
        case 0x11:
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          unaff_SS = 0x6b12;
          goto LAB_1008_67da;
        case 4:
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          unaff_SS = 0x6b28;
          goto LAB_1008_67da;
        case 0xe:
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          unaff_SS = 0x6b1c;
          goto LAB_1008_67da;
        case 0x26:
        case 0x27:
        case 0x28:
        case 0x29:
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          unaff_SS = 0x6b02;
          goto LAB_1008_67da;
        case 0x2b:
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          unaff_SS = 0x6ae2;
          goto LAB_1008_67da;
        case 0x2c:
        case 0x2d:
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          unaff_SS = 0x6afa;
          goto LAB_1008_67da;
        }
      }
      piVar1 = piVar7 + 3;
      local_4 = piVar7[4];
      piVar7 = (int *)*piVar1;
    }
    uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
    uVar8 = (undefined2)((ulong)uVar3 >> 0x10);
    iVar6 = (int)uVar3;
    uVar3 = *(undefined4 *)(iVar6 + 8);
    uVar3 = *(undefined4 *)((int)uVar3 + local_10 * 4);
    uVar4 = FUN_1008_916e(iVar6,uVar8,*(undefined2 *)((int)uVar3 + param_3 * 2));
    if ((uVar4 & 1) == 0) {
      return;
    }
    uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
    uVar3 = *(undefined4 *)((int)uVar3 + 8);
    uVar3 = *(undefined4 *)((int)uVar3 + iVar5 * 4);
    iVar5 = *(int *)((int)uVar3 + param_3 * 2);
    if (iVar5 == 0) goto switchD_1008_63cd_caseD_2b;
    if ((0x22 < iVar5) && (!SBORROW2(iVar5,0x23))) {
      if (iVar5 == 0x24 || iVar5 + -0x23 < 1) {
        pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
        unaff_SS = 0x6b32;
        goto LAB_1008_67da;
      }
      if (2 < iVar5 + -0x24) {
        if (iVar5 == 0x2a || iVar5 + -0x27 < 3) {
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          unaff_SS = 0x6b4c;
          goto LAB_1008_67da;
        }
        if (iVar5 == 0x5e) goto switchD_1008_63cd_caseD_36;
        if (iVar5 == 0x7e) {
          pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
          unaff_SS = 0x6b7e;
          goto LAB_1008_67da;
        }
        if (4 < iVar5 + -0x7e) {
          if (iVar5 == 0x86 || iVar5 + -0x83 < 3) {
            pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
            unaff_SS = 0x6b66;
            goto LAB_1008_67da;
          }
          if ((0x1b < iVar5 + -0x86) && (iVar5 + -0xa2 < 9)) {
            pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
            unaff_SS = 0x6b76;
            goto LAB_1008_67da;
          }
        }
      }
    }
switchD_1008_63cd_caseD_23:
    pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
    unaff_SS = 0x6ade;
  }
  else {
    uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
    uVar3 = *(undefined4 *)((int)uVar3 + 8);
    uVar3 = *(undefined4 *)((int)uVar3 + param_2 * 4);
    switch(*(undefined2 *)((int)uVar3 + param_3 * 2)) {
    case 2:
    case 0x87:
    case 0x88:
    case 0x89:
    case 0x8a:
    case 0x8b:
    case 0x8c:
    case 0x8d:
    case 0x8e:
    case 0x8f:
    case 0x90:
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:
    case 0x97:
    case 0x98:
    case 0x99:
    case 0x9a:
    case 0x9b:
    case 0x9c:
    case 0x9d:
    case 0x9e:
    case 0x9f:
    case 0xa0:
    case 0xa1:
      uVar8 = (undefined2)((ulong)*param_1 >> 0x10);
      iVar5 = (int)*param_1;
      puVar2 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(iVar5 + 0xdc) + 4);
      (*(code *)*puVar2)((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)(iVar5 + 0xdc),
                         *(undefined2 *)(iVar5 + 0xde),param_3,param_2);
      return;
    case 3:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x697c;
      break;
    case 4:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x69a2;
      break;
    case 5:
    case 6:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6982;
      break;
    case 7:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x69c0;
      break;
    case 8:
    case 9:
    case 10:
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
    case 0x18:
    case 0x1a:
    case 0x1c:
    case 0x1e:
    case 0x20:
    case 0x21:
    case 0x22:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x69e2;
      break;
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x19:
    case 0x1b:
    case 0x1d:
    case 0x1f:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x69c6;
      break;
    default:
      goto switchD_1008_63cd_caseD_23;
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0x2e:
    case 0x2f:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
switchD_1008_63cd_caseD_2b:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a32;
      break;
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
    case 0x3a:
    case 0x3b:
    case 0x3c:
    case 0x3d:
    case 0x3e:
    case 0x3f:
    case 0x40:
    case 0x5d:
    case 0x5f:
    case 0x60:
switchD_1008_63cd_caseD_36:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a2c;
      break;
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a38;
      break;
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
    case 0x50:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a44;
      break;
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
    case 0x58:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a20;
      break;
    case 0x59:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a16;
      break;
    case 0x5a:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x69fe;
      break;
    case 0x5b:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a08;
      break;
    case 0x5c:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x69f2;
      break;
    case 0x61:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a64;
      break;
    case 0x62:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a72;
      break;
    case 99:
    case 100:
    case 0x65:
    case 0x66:
    case 0x67:
    case 0x68:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a86;
      break;
    case 0x6d:
    case 0x6e:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a78;
      break;
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7a:
    case 0x7b:
    case 0x7c:
    case 0x7d:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a94;
      break;
    case 0x7f:
    case 0x80:
    case 0x81:
    case 0x82:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6ab6;
      break;
    case 0xab:
    case 0xac:
    case 0xad:
    case 0xae:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6a4c;
      break;
    case 0xaf:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6ac6;
      break;
    case 0xb0:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6ad2;
      break;
    case 0xb1:
      pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
      unaff_SS = 0x6ad8;
    }
  }
LAB_1008_67da:
  FUN_1008_67e6((undefined4 *)param_1,param_1._2_2_,unaff_SS,pcVar9);
  return;
}



/* ============================================================
   1008:67e6  FUN_1008_67e6
   ============================================================ */

void __stdcall16far
FUN_1008_67e6(undefined4 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  undefined2 uVar2;
  
  uVar2 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar1 = (int)*param_1;
  FUN_1008_55e0(*(undefined2 *)(iVar1 + 0x10),*(undefined2 *)(iVar1 + 0x12),param_3,param_4);
  uVar2 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar1 = (int)*param_1;
  FUN_1008_554e(*(undefined2 *)(iVar1 + 0x10),*(undefined2 *)(iVar1 + 0x12));
  return;
}



/* ============================================================
   1008:6b88  MainWindowProcedure
   ============================================================ */

void __stdcall16far
MainWindowProcedure(undefined2 param_1,undefined2 param_2,int param_3,int param_4,undefined2 param_5
                   )

{
  undefined2 *puVar1;
  char *unaff_CS;
  undefined2 unaff_SS;
  long lVar2;
  
  if (param_4 == 1) {
    FUN_1008_6e4c(*(undefined2 *)&DAT_1080_1026,*(undefined2 *)&DAT_1080_1028,param_5);
  }
  else if (param_4 == 2) {
    FUN_1008_6f3c(*(undefined2 *)&DAT_1080_1026,*(undefined2 *)&DAT_1080_1028);
  }
  else if ((param_4 == 3) || (param_4 == 0x1c)) {
    FUN_1008_a5fa(0x1080);
  }
  else if (param_4 == 0x100) {
    FUN_1008_6f46(*(undefined2 *)&DAT_1080_1026,*(undefined2 *)&DAT_1080_1028,param_1,param_2,
                  param_3);
  }
  else {
    if (param_4 == 0x104) {
      unaff_CS = (char *)s_Lucky_Chip_1080_1004 + 4;
      lVar2 = FUN_1008_73ce(*(undefined2 *)&DAT_1080_1026,*(undefined2 *)&DAT_1080_1028,param_1,
                            param_2,param_3);
      if (lVar2 == 0) {
        return;
      }
    }
    else if (param_4 == 0x112) {
      if (param_3 == -0x1667) {
        FUN_1008_3f18(*(undefined2 *)((int)*(undefined4 *)&DAT_1080_1026 + 4),
                      **(undefined2 **)*(undefined4 *)&DAT_1080_1026);
        return;
      }
      if (param_3 == -0x1668) {
        puVar1 = (undefined2 *)*(undefined4 *)&DAT_1080_1026;
        FUN_1008_2dfc(*puVar1,((undefined2 *)puVar1)[1]);
        return;
      }
    }
    DEFWINDOWPROC(unaff_CS,param_1,param_2,param_3,param_4);
  }
  return;
}



/* ============================================================
   1008:6c6a  FUN_1008_6c6a
   ============================================================ */

undefined2 * __stdcall16far
FUN_1008_6c6a(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  *param_1 = param_3;
  param_1[1] = param_4;
  param_1[2] = 0;
  param_1[6] = 0;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[3] = 0;
  if (DAT_1080_1028 != 0 || DAT_1080_1026 != (undefined2 *)0x0) {
    FUN_1008_2d02(*param_1,param_1[1],0,0x7430,(char *)s_Lucky_Chip_1080_1004 + 4);
  }
  DAT_1080_1026 = param_1;
  DAT_1080_1028 = param_2;
  FUN_1008_6dcc(param_1,param_2);
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1008:6cd8  FUN_1008_6cd8
   ============================================================ */

void __stdcall16far FUN_1008_6cd8(void)

{
  return;
}



/* ============================================================
   1008:6cdc  FUN_1008_6cdc
   ============================================================ */

void __stdcall16far FUN_1008_6cdc(undefined4 *param_1)

{
  uint uVar1;
  undefined2 uVar2;
  undefined2 in_DX;
  int iVar3;
  undefined2 uVar4;
  
  uVar1 = FUN_1000_1710(0x102a,0x1080);
  uVar2 = FUN_1000_1646((uVar1 >> 1) + 1);
  FUN_1008_40c0(0x102a,0x1080,uVar2,in_DX);
  uVar4 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar3 = (int)*param_1;
  FUN_1008_67e6(*(undefined2 *)(iVar3 + 0xc),*(undefined2 *)(iVar3 + 0xe),uVar2,in_DX);
  FUN_1000_1634(uVar2,in_DX);
  return;
}



/* ============================================================
   1008:6d30  FUN_1008_6d30
   ============================================================ */

bool __stdcall16far
FUN_1008_6d30(undefined4 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  int iVar1;
  undefined2 uVar2;
  undefined4 *puVar3;
  undefined2 unaff_BP;
  undefined2 uVar4;
  undefined2 unaff_CS;
  
  uVar4 = (undefined2)((ulong)param_1 >> 0x10);
  puVar3 = (undefined4 *)param_1;
  *(undefined2 *)((int)puVar3 + 6) = param_4;
  *(undefined2 *)(puVar3 + 2) = param_5;
  *(undefined2 *)((int)puVar3 + 10) = param_2;
  *(undefined2 *)(puVar3 + 3) = param_3;
  uVar2 = CREATEWINDOW(unaff_CS,0,0,*(undefined2 *)*param_1,0,0,0x1d4,0x27c,0x8000,0x8000,0,200,
                       0x384e,(char *)s_Lucky_Chip_1080_1004 + 4,0x745e,
                       (char *)s_Lucky_Chip_1080_1004 + 4,unaff_BP);
  *(undefined2 *)(puVar3 + 1) = uVar2;
  SHOWWINDOW(0x11d0,*(undefined2 *)((int)*param_1 + 2));
  UPDATEWINDOW(0x11d0,*(undefined2 *)(puVar3 + 1),uVar2);
  iVar1 = *(int *)(puVar3 + 1);
  if (iVar1 == 0) {
    FUN_1008_2d02(*(undefined2 *)param_1,*(undefined2 *)((int)puVar3 + 2),0,0x746a,
                  (char *)s_Lucky_Chip_1080_1004 + 4);
  }
  return iVar1 != 0;
}



/* ============================================================
   1008:6dcc  FUN_1008_6dcc
   ============================================================ */

undefined2 __stdcall16far FUN_1008_6dcc(undefined4 *param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 local_1c;
  undefined2 local_1a;
  char *local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  undefined2 local_c;
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_6;
  char *local_4;
  
  local_1c = 0;
  local_1a = 0x6b88;
  local_18 = (char *)s_Lucky_Chip_1080_1004 + 4;
  local_16 = 0;
  local_14 = 0;
  local_12 = *(undefined2 *)*param_1;
  uVar2 = *(undefined2 *)*param_1;
  local_10 = LOADICON(unaff_CS,0x748e,(char *)s_Lucky_Chip_1080_1004 + 4);
  uVar1 = 0;
  local_e = LOADCURSOR(0x11d0,0x7f00,0);
  local_c = GETSTOCKOBJECT(0x11d0,1,uVar1,uVar2);
  local_8 = 0;
  local_a = 0;
  local_6 = 0x745e;
  local_4 = (char *)s_Lucky_Chip_1080_1004 + 4;
  REGISTERCLASS(0x11d0,&local_1c);
  return 1;
}



/* ============================================================
   1008:6e4c  FUN_1008_6e4c
   ============================================================ */

void __stdcall16far FUN_1008_6e4c(undefined4 param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  undefined2 uVar3;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined1 local_e [4];
  int local_a;
  int local_8;
  int local_6;
  undefined2 local_4;
  
  GETSYSTEMMENU(unaff_CS,1);
  local_4 = GETSYSTEMMENU(0x11d0,0);
  APPENDMENU(0x11d0,0,0,0,0x800);
  uVar6 = local_4;
  APPENDMENU(0x11d0,0x7498,(char *)s_Lucky_Chip_1080_1004 + 4,0xe998,0);
  uVar5 = local_4;
  APPENDMENU(0x11d0,0,0,0,0x800);
  uVar4 = local_4;
  APPENDMENU(0x11d0,0x74ac,(char *)s_Lucky_Chip_1080_1004 + 4,0xe999,0);
  uVar3 = param_2;
  GETCLIENTRECT(0x11d0,local_e,unaff_SS);
  iVar1 = GETSYSTEMMETRICS(0x11d0,5,uVar3,uVar4,uVar5,uVar6);
  local_6 = (iVar1 + 0xf0) * 2;
  iVar1 = GETSYSTEMMETRICS(0x11d0,6);
  iVar1 = (iVar1 + 0xd8) * 2;
  uVar3 = (undefined2)((ulong)param_1 >> 0x10);
  iVar2 = (int)param_1;
  FUN_1008_4712(*(undefined2 *)(iVar2 + 6),*(undefined2 *)(iVar2 + 8),iVar1,0x78,
                local_8 / 2 - iVar1 / 2,0xc,param_2);
  FUN_1008_5dac(*(undefined2 *)(iVar2 + 10),*(undefined2 *)(iVar2 + 0xc),iVar1,local_6,
                local_8 / 2 - iVar1 / 2,(local_a - local_6) + -6,param_2);
  return;
}



/* ============================================================
   1008:6f3c  FUN_1008_6f3c
   ============================================================ */

void __stdcall16far FUN_1008_6f3c(void)

{
  POSTQUITMESSAGE();
  return;
}



/* ============================================================
   1008:6f46  FUN_1008_6f46
   ============================================================ */

void __stdcall16far FUN_1008_6f46(undefined4 *param_1,int param_2,int param_3,int param_4)

{
  int *piVar1;
  undefined2 *puVar2;
  undefined4 uVar3;
  bool bVar4;
  uint uVar5;
  char extraout_AH;
  char extraout_AH_00;
  int iVar6;
  int iVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  char *unaff_CS;
  undefined2 unaff_SS;
  undefined2 local_a;
  undefined2 local_8;
  
  uVar8 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar6 = (int)*param_1;
  piVar1 = (int *)*(int *)(iVar6 + 0xce);
  iVar6 = *(int *)(iVar6 + 0xd0);
  if (iVar6 == 0 && piVar1 == (int *)0x0) {
    uVar8 = 0x74b6;
    goto LAB_1008_73b5;
  }
  if (piVar1[4] != DAT_1080_2992) {
    DAT_1080_106e = 0;
  }
  DAT_1080_2992 = param_4;
  uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
  if (*(int *)((int)uVar3 + 0xc) != 0) {
    if ((param_4 == 0) && (param_3 == 0 && param_2 == 0)) {
      puVar2 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)((int)*param_1 + 0xdc) + 0x30);
      param_4 = (*(code *)*puVar2)();
    }
    else if ((param_4 != 0x70) && ((param_4 != 0x4e && (param_4 != 0x4c)))) {
      return;
    }
  }
  uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
  if (*(int *)((int)uVar3 + 0xc) == 0) {
    unaff_CS = (char *)0x11d0;
    GETASYNCKEYSTATE();
    uVar5 = (uint)(extraout_AH != '\0');
  }
  else {
    puVar2 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)((int)*param_1 + 0xdc) + 0x30);
    uVar5 = (*(code *)*puVar2)();
  }
  uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
  if (*(int *)((int)uVar3 + 0xc) == 0) {
    unaff_CS = (char *)0x11d0;
    GETASYNCKEYSTATE();
  }
  bVar4 = false;
  local_8 = 0;
  local_a = 0;
  uVar8 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar7 = (int)*param_1;
  uVar3 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 0xdc) + 8);
  uVar3 = *(undefined4 *)((int)uVar3 + *(int *)(iVar7 + 0xea) * 4);
  iVar7 = *(int *)((int)uVar3 + *(int *)(iVar7 + 0xec) * 2);
  if (*piVar1 == param_4) {
    local_8 = 0;
    local_a = 0xffff;
  }
  else {
    if (piVar1[1] == param_4) {
      local_8 = 0;
      goto LAB_1008_70b0;
    }
    if (piVar1[2] == param_4) {
      local_a = 0;
LAB_1008_70c0:
      local_8 = 1;
    }
    else if (piVar1[3] == param_4) {
      local_8 = 0xffff;
      local_a = 0;
    }
    else if (piVar1[5] == param_4) {
      local_a = 0xffff;
      local_8 = local_a;
    }
    else {
      if (piVar1[7] == param_4) {
        local_8 = 0xffff;
      }
      else {
        if (piVar1[4] == param_4) {
          if (((*(int *)((int)*param_1 + 0xea) == 0x27) && (*(int *)((int)*param_1 + 0xec) == 0)) &&
             (uVar5 == 1)) {
            *(undefined2 *)&DAT_1080_106e = 1;
          }
          local_a = 0xffff;
          goto LAB_1008_70c0;
        }
        if (piVar1[6] != param_4) {
          if (piVar1[8] != param_4) {
            if (piVar1[0xb] == param_4) {
              FUN_1008_2c84(*(undefined2 *)param_1,*(undefined2 *)((int)(undefined4 *)param_1 + 2));
              return;
            }
            if (piVar1[0xc] == param_4) {
              uVar8 = (undefined2)((ulong)*param_1 >> 0x10);
              iVar6 = (int)*param_1;
              FUN_1008_55be(*(undefined2 *)(iVar6 + 0x10),*(undefined2 *)(iVar6 + 0x12));
              return;
            }
            if (piVar1[0xd] == param_4) {
              uVar8 = (undefined2)((ulong)*param_1 >> 0x10);
              iVar6 = (int)*param_1;
              FUN_1008_5588(*(undefined2 *)(iVar6 + 0x10),*(undefined2 *)(iVar6 + 0x12));
              return;
            }
            if (piVar1[0xf] == param_4) {
              FUN_1008_134e(*(undefined2 *)param_1,*(undefined2 *)((int)(undefined4 *)param_1 + 2));
            }
            else {
              if (piVar1[0xe] == param_4) {
                FUN_1008_1efe(*(undefined2 *)param_1,*(undefined2 *)((int)(undefined4 *)param_1 + 2)
                             );
                goto LAB_1008_71b1;
              }
              if (piVar1[9] == param_4) {
                FUN_1008_29e8(*(undefined2 *)param_1,*(undefined2 *)((int)(undefined4 *)param_1 + 2)
                             );
                return;
              }
              if (piVar1[10] == param_4) {
                FUN_1008_2a98(*(undefined2 *)param_1,*(undefined2 *)((int)(undefined4 *)param_1 + 2)
                             );
                return;
              }
              if (piVar1[0x10] == param_4) {
                if (*(int *)((int)*param_1 + 0xe0) == 1) {
                  *(undefined2 *)((int)*param_1 + 0xe0) = 0;
                  uVar8 = 0x74e4;
                }
                else {
                  *(undefined2 *)((int)*param_1 + 0xe0) = 1;
                  uVar8 = 0x74ee;
                }
LAB_1008_73b5:
                uVar9 = (undefined2)((ulong)*param_1 >> 0x10);
                iVar6 = (int)*param_1;
                FUN_1008_67e6(*(undefined2 *)(iVar6 + 0xc),*(undefined2 *)(iVar6 + 0xe),uVar8,
                              (char *)s_Lucky_Chip_1080_1004 + 4);
                return;
              }
              if (piVar1[0x12] == param_4) {
                if (*(int *)&DAT_1080_106e == 1) {
                  unaff_CS = (char *)s_Lucky_Chip_1080_1004 + 4;
                  FUN_1008_6cdc((undefined4 *)param_1,param_1._2_2_);
                }
                if (iVar7 < 0x27) {
                  return;
                }
                if (0x2a < iVar7) {
                  return;
                }
                uVar8 = (undefined2)((ulong)*param_1 >> 0x10);
                iVar6 = (int)*param_1;
                puVar2 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(iVar6 + 0xdc) + 0x14);
                (*(code *)*puVar2)(unaff_CS,*(undefined2 *)(iVar6 + 0xdc),
                                   *(undefined2 *)(iVar6 + 0xde),iVar7);
                return;
              }
              if (piVar1[0x11] == param_4) {
                if (iVar7 < 0x23) {
                  return;
                }
                if (0x26 < iVar7) {
                  return;
                }
                uVar8 = (undefined2)((ulong)*param_1 >> 0x10);
                iVar6 = (int)*param_1;
                puVar2 = (undefined2 *)((int)*(undefined4 *)*(undefined4 *)(iVar6 + 0xdc) + 0x18);
                (*(code *)*puVar2)(unaff_CS,*(undefined2 *)(iVar6 + 0xdc),
                                   *(undefined2 *)(iVar6 + 0xde),iVar7);
                return;
              }
              if (param_4 == 0x4e) {
                uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
                if (*(int *)((int)uVar3 + 0xc) != 0) {
                  GETASYNCKEYSTATE(unaff_CS,0x10);
                  *(uint *)((int)*param_1 + 0xe2) = (uint)(extraout_AH_00 != '\0');
                  *(undefined2 *)((int)*param_1 + 0xe2) = 0;
                  FUN_1008_0630(*(undefined2 *)param_1,
                                *(undefined2 *)((int)(undefined4 *)param_1 + 2));
LAB_1008_732a:
                  FUN_1008_a5fa();
                  *(undefined2 *)&DAT_1080_1070 = 1;
                  uVar8 = (undefined2)((ulong)*param_1 >> 0x10);
                  iVar6 = (int)*param_1;
                  FUN_1008_a652(*(undefined2 *)(iVar6 + 0xdc),*(undefined2 *)(iVar6 + 0xde));
                  INVALIDATERECT((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,
                                 *(undefined2 *)((undefined4 *)param_1 + 1));
                  return;
                }
                uVar8 = 0x74f8;
                goto LAB_1008_73b5;
              }
              if (param_4 == 0x4c) {
                uVar3 = *(undefined4 *)((int)*param_1 + 0xdc);
                if (*(int *)((int)uVar3 + 0xc) != 0) {
                  FUN_1008_0a3c(*(undefined2 *)param_1,
                                *(undefined2 *)((int)(undefined4 *)param_1 + 2));
                  goto LAB_1008_732a;
                }
                uVar8 = 0x752c;
                goto LAB_1008_73b5;
              }
              if (param_4 == 0x70) {
                *(undefined2 *)&DAT_1080_1bf0 = 1;
              }
              else if (param_4 == 0x52) {
                *(undefined2 *)&DAT_1080_1bf0 = 0xb;
              }
              else {
                if (param_4 != 0x43) goto LAB_1008_7080;
                *(undefined2 *)&DAT_1080_1bf0 = 6;
              }
              FUN_1008_2dfc(*(undefined2 *)param_1,*(undefined2 *)((int)(undefined4 *)param_1 + 2));
            }
LAB_1008_71b1:
            FUN_1008_a5fa();
            return;
          }
          local_a = 0;
          local_8 = local_a;
          goto LAB_1008_707e;
        }
        local_8 = 1;
      }
LAB_1008_70b0:
      local_a = 1;
    }
  }
LAB_1008_707e:
  bVar4 = true;
LAB_1008_7080:
  if (bVar4) {
    FUN_1008_1b22(*(undefined2 *)param_1,*(undefined2 *)((int)(undefined4 *)param_1 + 2),uVar5,
                  local_a,local_8);
  }
  return;
}



/* ============================================================
   1008:73ce  FUN_1008_73ce
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_73ce(undefined4 *param_1,undefined2 param_2,undefined2 param_3,uint param_4,int param_5)

{
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)((int)*param_1 + 0xdc);
  if (*(int *)((int)uVar1 + 0xc) == 0) {
    if (param_5 == 0x26) {
      if ((param_4 & 0x2000) != 0) {
        FUN_1008_29e8(*(undefined2 *)param_1,*(undefined2 *)((int)param_1 + 2));
        return 0;
      }
    }
    else if (param_5 != 0x28) {
      return 1;
    }
    if ((param_4 & 0x2000) != 0) {
      FUN_1008_2a98(*(undefined2 *)param_1,*(undefined2 *)((int)param_1 + 2));
      return 0;
    }
  }
  return 1;
}



/* ============================================================
   1008:7560  FUN_1008_7560
   ============================================================ */

undefined2 * __stdcall16far
FUN_1008_7560(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,int param_6)

{
  undefined2 uVar1;
  undefined2 *puVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  undefined4 uVar6;
  
  *param_1 = 0xd490;
  param_1[1] = (char *)s_Lucky_Chip_1080_1004 + 4;
  param_1[2] = param_5;
  param_1[3] = param_6;
  uVar1 = FUN_1000_1646(0xa0);
  param_1[4] = uVar1;
  param_1[5] = param_6;
  for (iVar4 = 0; iVar4 < 0x28; iVar4 = iVar4 + 1) {
    uVar1 = FUN_1000_1646(0x48);
    uVar5 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
    puVar2 = (undefined2 *)((int)*(undefined4 *)(param_1 + 4) + iVar4 * 4);
    *puVar2 = uVar1;
    puVar2[1] = param_6;
  }
  for (iVar4 = 0; iVar4 < 0x28; iVar4 = iVar4 + 1) {
    for (iVar3 = 0; iVar3 < 0x24; iVar3 = iVar3 + 1) {
      param_6 = iVar4 * 4;
      uVar6 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + param_6);
      *(undefined2 *)((int)uVar6 + iVar3 * 2) = 0;
    }
  }
  param_1[8] = 0;
  param_1[7] = 0;
  param_1[9] = param_3;
  iVar4 = 0;
  do {
    param_1[iVar4 * 2 + 0xb] = 0;
    param_1[iVar4 * 2 + 10] = 0;
    iVar4 = iVar4 + 1;
  } while (iVar4 < 4);
  param_1[0x20] = 0;
  param_1[0x1f] = 0;
  param_1[0x24] = 0;
  param_1[0x23] = 0;
  param_1[0x22] = 0;
  param_1[0x21] = 0;
  param_1[0x25] = 10;
  param_1[0x26] = 1;
  param_1[0x27] = 0x14;
  iVar4 = FUN_1000_1646(6);
  if (param_6 == 0 && iVar4 == 0) {
    uVar6 = 0;
  }
  else {
    uVar6 = FUN_1008_aefa(iVar4,param_6);
  }
  iVar4 = (int)((ulong)uVar6 >> 0x10);
  param_1[0x1b] = (int)uVar6;
  param_1[0x1c] = iVar4;
  if (param_1[0x1c] == 0 && param_1[0x1b] == 0) {
    uVar1 = 0xd000;
  }
  else {
    param_1[0x18] = 10;
    uVar1 = FUN_1000_1646(0x78);
    param_1[0x16] = uVar1;
    param_1[0x17] = iVar4;
    if (iVar4 == 0 && param_1[0x16] == 0) {
      uVar1 = 0xd02c;
    }
    else {
      param_1[0x28] = 0;
      uVar1 = FUN_1000_1646(0x5dc);
      param_1[0x19] = uVar1;
      param_1[0x1a] = iVar4;
      if (iVar4 != 0 || param_1[0x19] != 0) {
        param_1[0x29] = 0;
        param_1[0x1e] = param_4;
        param_1[6] = 0;
        goto LAB_1008_7725;
      }
      uVar1 = 0xd054;
    }
  }
  FUN_1008_2d02(param_1[2],param_1[3],0,uVar1,(char *)s_Lucky_Chip_1080_1004 + 4);
LAB_1008_7725:
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1008:772c  FUN_1008_772c
   ============================================================ */

void __stdcall16far FUN_1008_772c(undefined2 *param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 *puVar2;
  int iVar3;
  undefined2 uVar4;
  
  *param_1 = 0xd490;
  param_1[1] = (char *)s_Lucky_Chip_1080_1004 + 4;
  iVar3 = 0;
  do {
    uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
    puVar2 = (undefined2 *)((int)*(undefined4 *)(param_1 + 4) + iVar3 * 4);
    FUN_1000_1634(*puVar2,puVar2[1]);
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x28);
  FUN_1000_1634(param_1[4],param_1[5]);
  FUN_1000_1634(param_1[0x16],param_1[0x17]);
  FUN_1000_1634(param_1[0x19],param_1[0x1a]);
  iVar3 = param_1[0x1b];
  iVar1 = param_1[0x1c];
  if (iVar1 != 0 || iVar3 != 0) {
    FUN_1008_af18(iVar3,iVar1);
    FUN_1000_1634(iVar3,iVar1);
  }
  return;
}



/* ============================================================
   1008:77d0  FUN_1008_77d0
   ============================================================ */

void __stdcall16far FUN_1008_77d0(undefined4 *param_1,undefined2 param_2)

{
  byte *pbVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined2 *puVar6;
  undefined4 uVar7;
  bool bVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  uint uVar13;
  int *piVar14;
  int iVar15;
  int iVar16;
  undefined2 uVar17;
  int local_a;
  
  DAT_1080_1070 = 1;
  iVar16 = 0;
  do {
    if (*(int *)((int)param_1 + 0x52) <= iVar16) {
      return;
    }
    uVar17 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x32) >> 0x10);
    piVar14 = (int *)((int)*(undefined4 *)((int)param_1 + 0x32) + iVar16 * 10);
    iVar3 = *piVar14;
    FUN_1008_a060(param_1,param_2,piVar14[2],piVar14[1]);
    uVar17 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x32) >> 0x10);
    iVar15 = (int)*(undefined4 *)((int)param_1 + 0x32) + iVar16 * 10;
    iVar15 = FUN_1008_91c6(param_1,param_2,*(undefined2 *)(iVar15 + 4),*(undefined2 *)(iVar15 + 2));
    if (iVar15 != 0) {
      uVar17 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x32) >> 0x10);
      iVar15 = (int)*(undefined4 *)((int)param_1 + 0x32) + iVar16 * 10;
      pbVar1 = (byte *)((*(int *)(iVar15 + 2) * 0x24 + *(int *)(iVar15 + 4)) * 2);
      *pbVar1 = *pbVar1 & 0xfb;
    }
    iVar9 = iVar16 * 10;
    uVar17 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x32) >> 0x10);
    iVar15 = (int)*(undefined4 *)((int)param_1 + 0x32) + iVar9;
    piVar2 = (int *)(iVar15 + 2);
    *piVar2 = *piVar2 + *(int *)(iVar15 + 6);
    uVar17 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x32) >> 0x10);
    iVar15 = (int)*(undefined4 *)((int)param_1 + 0x32) + iVar9;
    piVar2 = (int *)(iVar15 + 4);
    *piVar2 = *piVar2 + *(int *)(iVar15 + 8);
    uVar17 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x32) >> 0x10);
    iVar9 = (int)*(undefined4 *)((int)param_1 + 0x32) + iVar9;
    iVar15 = *(int *)(iVar9 + 2);
    iVar9 = *(int *)(iVar9 + 4);
    iVar10 = FUN_1008_91c6(param_1,param_2,iVar9,iVar15);
    if (iVar10 == 0) {
      FUN_1008_7f8e(param_1,param_2,iVar16);
LAB_1008_7d9f:
      iVar16 = iVar16 + -1;
    }
    else {
      FUN_1008_a060(param_1,param_2,iVar9,iVar15);
      pbVar1 = (byte *)((iVar15 * 0x24 + iVar9) * 2);
      *pbVar1 = *pbVar1 | 4;
      for (local_a = 0; local_a < *(int *)(param_1 + 0x14); local_a = local_a + 1) {
        uVar17 = (undefined2)((ulong)param_1[0xb] >> 0x10);
        piVar14 = (int *)((int)param_1[0xb] + local_a * 0xc);
        iVar10 = piVar14[1];
        iVar4 = piVar14[2];
        iVar5 = *piVar14;
        if (((iVar3 == 4) && (iVar11 = FUN_1000_1f42(iVar10 - iVar15), iVar11 < 2)) &&
           (iVar11 = FUN_1000_1f42(iVar4 - iVar9), iVar11 < 2)) {
          iVar11 = FUN_1000_1f42(iVar4 - iVar9);
          iVar12 = FUN_1000_1f42(iVar10 - iVar15);
          if (((iVar12 + iVar11 != 0) && (iVar5 != 0)) &&
             ((iVar5 != 4 &&
              ((iVar5 != 1 && (iVar11 = FUN_1008_3c88(iVar12 + iVar11 + 1), iVar11 != 1))))))
          goto LAB_1008_79f9;
          FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),0x14,
                        iVar4,iVar10);
          FUN_1008_31d4(*(undefined2 *)(param_1 + 1),*(undefined2 *)((int)param_1 + 6),0xd);
          if ((iVar5 == 2) && (iVar11 = FUN_1008_3c88(10), iVar11 == 2)) {
            FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),1,
                          iVar4,iVar10);
          }
          if (iVar5 == 3) {
            FUN_1008_bc2c(param_1,param_2,iVar4,iVar10);
          }
          FUN_1008_ad16(param_1,param_2,iVar5);
          FUN_1008_8032(param_1,param_2,local_a);
          local_a = local_a + -1;
        }
        else {
LAB_1008_79f9:
          if ((iVar10 == iVar15) && (iVar4 == iVar9)) {
            if (iVar3 == 0) {
              if (((iVar5 == 0) || (iVar5 == 1)) || (iVar5 == 4)) {
                if ((iVar5 == 0) || (iVar5 == 1)) {
                  FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),
                                0x16,iVar9,iVar15);
                  FUN_1008_31d4(*(undefined2 *)(param_1 + 1),*(undefined2 *)((int)param_1 + 6),9);
                  uVar7 = *(undefined4 *)((int)param_1[2] + iVar15 * 4);
                  if (*(int *)((int)uVar7 + iVar9 * 2) == 0) {
                    if (iVar5 == 1) {
                      uVar7 = *(undefined4 *)((int)param_1[2] + iVar15 * 4);
                      *(undefined2 *)((int)uVar7 + iVar9 * 2) = 4;
                    }
                    else {
                      iVar10 = FUN_1008_ced2(param_1,param_2);
                      if (iVar10 < 4) {
                        uVar17 = 5;
                      }
                      else {
                        uVar17 = 6;
                      }
                      uVar7 = *(undefined4 *)((int)param_1[2] + iVar15 * 4);
                      *(undefined2 *)((int)uVar7 + iVar9 * 2) = uVar17;
                    }
                  }
                }
                else {
                  FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),
                                0x17,iVar9,iVar15);
                  FUN_1008_31d4(*(undefined2 *)(param_1 + 1),*(undefined2 *)((int)param_1 + 6),6);
                }
                goto LAB_1008_7b8e;
              }
              if ((iVar5 != 2) && (iVar5 != 3)) goto LAB_1008_7a2d;
            }
            else {
LAB_1008_7a2d:
              if (iVar3 != 1) {
                if (iVar3 != 2) goto LAB_1008_7a42;
                FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),
                              0x14,iVar9,iVar15);
                FUN_1008_31d4(*(undefined2 *)(param_1 + 1),*(undefined2 *)((int)param_1 + 6),0xc);
                if ((iVar5 == 2) && (iVar10 = FUN_1008_3c88(10), iVar10 == 2)) {
                  FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),
                                1,iVar9,iVar15);
                }
                if (iVar5 == 3) {
                  FUN_1008_bc2c(param_1,param_2,iVar9,iVar15);
                }
                FUN_1008_ad16(param_1,param_2,iVar5);
                FUN_1008_8032(param_1,param_2,local_a);
                FUN_1008_7f8e(param_1,param_2,iVar16);
                FUN_1008_7f48(param_1,param_2,iVar9,iVar15);
                bVar8 = true;
                goto LAB_1008_7c60;
              }
              FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),0x14
                            ,iVar9,iVar15);
              FUN_1008_31d4(*(undefined2 *)(param_1 + 1),*(undefined2 *)((int)param_1 + 6),0xc);
              if ((iVar5 == 2) && (iVar10 = FUN_1008_3c88(10), iVar10 == 2)) {
                FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),1,
                              iVar9,iVar15);
              }
              if (iVar5 == 3) {
                FUN_1008_bc2c(param_1,param_2,iVar9,iVar15);
              }
LAB_1008_7b8e:
              FUN_1008_ad16(param_1,param_2,iVar5);
              FUN_1008_8032(param_1,param_2,local_a);
            }
            FUN_1008_7f8e(param_1,param_2,iVar16);
            bVar8 = true;
            goto LAB_1008_7c60;
          }
        }
LAB_1008_7a42:
      }
      bVar8 = false;
LAB_1008_7c60:
      if (bVar8) goto LAB_1008_7d9f;
      iVar10 = FUN_1008_917e(param_1,param_2,iVar9,iVar15);
      bVar8 = false;
      if (iVar10 == 0) {
        if ((iVar3 == 0) || (iVar3 == 4)) {
          FUN_1008_7f8e(param_1,param_2,iVar16);
          bVar8 = true;
        }
        if (iVar3 == 1) {
          uVar7 = *(undefined4 *)((int)param_1[2] + iVar15 * 4);
          uVar13 = FUN_1008_916e(param_1,param_2,*(undefined2 *)((int)uVar7 + iVar9 * 2));
          if ((uVar13 & 4) == 0) {
            puVar6 = (undefined2 *)((int)*param_1 + 0x40);
            (*(code *)*puVar6)((char *)s_Lucky_Chip_1080_1004 + 4,param_1,param_2,iVar9,iVar15,1);
          }
          else {
            FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),0x14,
                          iVar9,iVar15);
            FUN_1008_31d4(*(undefined2 *)(param_1 + 1),*(undefined2 *)((int)param_1 + 6),0xc);
          }
          FUN_1008_7f8e(param_1,param_2,iVar16);
          bVar8 = true;
        }
        if (iVar3 == 2) {
          uVar7 = *(undefined4 *)((int)param_1[2] + iVar15 * 4);
          uVar13 = FUN_1008_916e(param_1,param_2,*(undefined2 *)((int)uVar7 + iVar9 * 2));
          if ((uVar13 & 4) == 0) {
            puVar6 = (undefined2 *)((int)*param_1 + 0x40);
            (*(code *)*puVar6)((char *)s_Lucky_Chip_1080_1004 + 4,param_1,param_2,iVar9,iVar15,2);
          }
          else {
            FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),0x14,
                          iVar9,iVar15);
            FUN_1008_31d4(*(undefined2 *)(param_1 + 1),*(undefined2 *)((int)param_1 + 6),0xc);
          }
          FUN_1008_7f8e(param_1,param_2,iVar16);
          bVar8 = true;
        }
      }
      if (bVar8) goto LAB_1008_7d9f;
    }
    iVar16 = iVar16 + 1;
  } while( true );
}



/* ============================================================
   1008:7f48  FUN_1008_7f48
   ============================================================ */

void __stdcall16far
FUN_1008_7f48(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  undefined2 local_4;
  
  local_4 = -1;
  do {
    for (iVar1 = -1; iVar1 < 2; iVar1 = iVar1 + 1) {
      if ((local_4 != 0) || (iVar1 != 0)) {
        FUN_1008_8e22(param_1,param_2,iVar1,local_4,param_3,param_4,1);
      }
    }
    local_4 = local_4 + 1;
  } while (local_4 < 2);
  return;
}



/* ============================================================
   1008:7f8e  FUN_1008_7f8e
   ============================================================ */

void __stdcall16far FUN_1008_7f8e(undefined4 param_1,int param_2)

{
  byte *pbVar1;
  undefined2 *puVar2;
  undefined2 *puVar3;
  int iVar4;
  undefined2 *puVar5;
  int iVar6;
  int iVar7;
  undefined2 *puVar8;
  undefined2 uVar9;
  int local_4;
  
  if (*(int *)((int)param_1 + 0x52) == 0) {
    FUN_1008_2d02(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0,0xd0ce,
                  (char *)s_Lucky_Chip_1080_1004 + 4);
  }
  else {
    uVar9 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x32) >> 0x10);
    iVar7 = (int)*(undefined4 *)((int)param_1 + 0x32) + param_2 * 10;
    iVar6 = *(int *)(iVar7 + 2);
    iVar7 = *(int *)(iVar7 + 4);
    iVar4 = FUN_1008_91c6((int)param_1,param_1._2_2_,iVar7,iVar6);
    if (iVar4 != 0) {
      pbVar1 = (byte *)((iVar6 * 0x24 + iVar7) * 2);
      *pbVar1 = *pbVar1 & 0xfb;
    }
    local_4 = param_2;
    while( true ) {
      if (*(int *)((int)param_1 + 0x52) + -1 <= local_4) break;
      uVar9 = *(undefined2 *)((int)param_1 + 0x34);
      puVar5 = (undefined2 *)(*(int *)((int)param_1 + 0x32) + local_4 * 10);
      puVar8 = puVar5 + 5;
      for (iVar6 = 5; iVar6 != 0; iVar6 = iVar6 + -1) {
        puVar3 = puVar5;
        puVar5 = puVar5 + 1;
        puVar2 = puVar8;
        puVar8 = puVar8 + 1;
        *puVar3 = *puVar2;
      }
      local_4 = local_4 + 1;
    }
    *(int *)((int)param_1 + 0x52) = *(int *)((int)param_1 + 0x52) + -1;
  }
  return;
}



/* ============================================================
   1008:8032  FUN_1008_8032
   ============================================================ */

void __stdcall16far FUN_1008_8032(undefined4 param_1,int param_2)

{
  byte *pbVar1;
  undefined2 *puVar2;
  undefined2 *puVar3;
  undefined2 *puVar4;
  undefined2 *puVar5;
  int iVar6;
  undefined2 uVar7;
  int local_4;
  
  if (*(int *)((int)param_1 + 0x50) == 0) {
    FUN_1008_2d02(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0,0xd106,
                  (char *)s_Lucky_Chip_1080_1004 + 4);
  }
  else {
    uVar7 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10);
    iVar6 = (int)*(undefined4 *)((int)param_1 + 0x2c) + param_2 * 0xc;
    pbVar1 = (byte *)((*(int *)(iVar6 + 2) * 0x24 + *(int *)(iVar6 + 4)) * 2);
    *pbVar1 = *pbVar1 & 0xfd;
    local_4 = param_2;
    while( true ) {
      if (*(int *)((int)param_1 + 0x50) + -1 <= local_4) break;
      uVar7 = *(undefined2 *)((int)param_1 + 0x2e);
      puVar4 = (undefined2 *)(*(int *)((int)param_1 + 0x2c) + local_4 * 0xc);
      puVar5 = puVar4 + 6;
      for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
        puVar3 = puVar4;
        puVar4 = puVar4 + 1;
        puVar2 = puVar5;
        puVar5 = puVar5 + 1;
        *puVar3 = *puVar2;
      }
      local_4 = local_4 + 1;
    }
    *(int *)((int)param_1 + 0x50) = *(int *)((int)param_1 + 0x50) + -1;
  }
  return;
}



/* ============================================================
   1008:80c0  FUN_1008_80c0
   ============================================================ */

void __stdcall16far
FUN_1008_80c0(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,int param_6,undefined2 param_7,undefined2 param_8,undefined2 param_9
             )

{
  int iVar1;
  undefined2 uVar2;
  int iVar3;
  
  do {
    if (param_6 < 1) {
      return;
    }
    iVar1 = *(int *)(param_1 + 0x30);
    if (*(int *)(param_1 + 0x50) < iVar1) {
      *(int *)(param_1 + 0x30) = iVar1;
    }
    else {
      if (299 < iVar1) {
        *(int *)(param_1 + 0x30) = iVar1;
        return;
      }
      iVar3 = iVar1 >> 0xf;
      iVar1 = iVar1 / 2 + iVar1;
      if (300 < iVar1) {
        iVar1 = 300;
      }
      uVar2 = FUN_1000_1646(iVar1 * 0xc);
      FUN_1000_1e38(uVar2,iVar3,*(undefined2 *)(param_1 + 0x2c),*(undefined2 *)(param_1 + 0x2e),
                    *(int *)(param_1 + 0x50) * 0xc);
      FUN_1000_1634(*(undefined2 *)(param_1 + 0x2c),*(undefined2 *)(param_1 + 0x2e));
      *(undefined2 *)(param_1 + 0x2c) = uVar2;
      *(int *)(param_1 + 0x2e) = iVar3;
      *(int *)(param_1 + 0x30) = iVar1;
    }
    iVar1 = *(int *)(param_1 + 0x50);
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 2) = param_8;
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 4) = param_7;
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc) = param_9;
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 10) = param_5;
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 6) = param_4;
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 8) = param_3;
    *(int *)(param_1 + 0x50) = iVar1 + 1;
    param_6 = param_6 + -1;
  } while( true );
}



/* ============================================================
   1008:81dc  FUN_1008_81dc
   ============================================================ */

void __stdcall16far FUN_1008_81dc(int param_1,undefined2 param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  int iVar5;
  undefined2 uVar6;
  int local_8;
  int local_6;
  
  if (*(int *)((int)*(undefined4 *)(param_1 + 4) + 0xfe) != 1) {
    for (local_6 = 0; local_6 < 0x28; local_6 = local_6 + 1) {
      iVar5 = 0;
      do {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + local_6 * 4);
        iVar3 = *(int *)((int)uVar1 + iVar5 * 2);
        if ((0x17 < iVar3) && (iVar3 < 0x23)) {
          local_8 = *(int *)(param_1 + 0x4a);
          if (iVar3 == 0x21) {
            local_8 = 0;
          }
          if (iVar3 == 0x22) {
            local_8 = *(int *)(param_1 + 0x4a) << 2;
          }
          if ((*(int *)(param_1 + 0x50) < 300) && (iVar2 = FUN_1008_3c88(100), iVar2 <= local_8)) {
            switch(iVar3) {
            case 0x18:
            case 0x19:
switchD_1008_8278_caseD_18:
              iVar3 = FUN_1008_3c88(10);
              if (iVar3 < 9) {
                uVar6 = 0;
              }
              else {
                uVar6 = 1;
              }
              break;
            case 0x1a:
            case 0x1b:
              iVar3 = FUN_1008_3c88(10);
              if (4 < iVar3) goto switchD_1008_8278_caseD_18;
LAB_1008_82b1:
              uVar6 = 2;
              break;
            case 0x1c:
            case 0x1d:
              iVar3 = FUN_1008_3c88(10);
              if (4 < iVar3) goto switchD_1008_8278_caseD_18;
              iVar3 = FUN_1008_3c88(10);
              if (3 < iVar3) goto LAB_1008_82b1;
              uVar6 = 3;
              break;
            default:
              goto switchD_1008_8278_caseD_1e;
            case 0x20:
            case 0x22:
              uVar6 = 4;
            }
            iVar3 = iVar5;
            iVar2 = local_6;
            uVar4 = FUN_1008_3c88(*(undefined2 *)(param_1 + 0x4c),iVar5,local_6,uVar6);
            FUN_1008_80c0(param_1,param_2,0,0,0,uVar4,iVar3,iVar2,uVar6);
          }
        }
switchD_1008_8278_caseD_1e:
        iVar5 = iVar5 + 1;
      } while (iVar5 < 0x24);
    }
  }
  return;
}



/* ============================================================
   1008:8310  FUN_1008_8310
   ============================================================ */

void __stdcall16far FUN_1008_8310(undefined4 param_1)

{
  byte *pbVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined2 uVar8;
  undefined2 unaff_SS;
  undefined1 local_1e [4];
  int local_1a;
  int local_18;
  int local_16;
  int local_14;
  int local_12;
  int local_10;
  int local_e;
  int local_c;
  int local_a;
  int local_8;
  int local_6;
  int local_4;
  
  FUN_1008_adee(local_1e,unaff_SS,(int)param_1,param_1._2_2_);
  DAT_1080_1070 = 1;
  FUN_1008_81dc((int)param_1,param_1._2_2_);
  iVar7 = 0;
  do {
    if (*(int *)((int)param_1 + 0x50) <= iVar7) {
      FUN_1008_aeba(local_1e,unaff_SS);
      return;
    }
    local_e = 0xfff;
    local_16 = 0;
    local_14 = 0;
    local_18 = 0;
    local_1a = 0;
    uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10);
    iVar6 = (int)*(undefined4 *)((int)param_1 + 0x2c) + iVar7 * 0xc;
    local_10 = *(int *)(iVar6 + 2);
    local_12 = *(int *)(iVar6 + 4);
    if (*(int *)(iVar6 + 10) == 0) {
      iVar6 = -1;
      local_c = iVar7;
      while (iVar6 < 2) {
        local_a = iVar6;
        for (iVar7 = -1; iVar7 < 2; iVar7 = iVar7 + 1) {
          local_4 = local_a + local_10;
          local_6 = local_12 + iVar7;
          iVar6 = FUN_1008_91c6((int)param_1,param_1._2_2_,local_6,local_4);
          if (iVar6 != 0) {
            if (((*(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + local_c * 0xc) == 1) ||
                (*(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + local_c * 0xc) == 3)) &&
               (((uVar4 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + local_4 * 4),
                 2 < *(int *)((int)uVar4 + local_6 * 2) &&
                 ((uVar4 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + local_4 * 4),
                  *(int *)((int)uVar4 + local_6 * 2) < 7 &&
                  (iVar6 = FUN_1008_3c88(*(undefined2 *)((int)param_1 + 0x4e)), iVar6 == 1)))) ||
                ((uVar4 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + local_4 * 4),
                 *(int *)((int)uVar4 + local_6 * 2) == 7 &&
                 (iVar6 = FUN_1008_3c88(*(int *)((int)param_1 + 0x4e) * 2), iVar6 == 1)))))) {
              FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                            *(undefined2 *)((int)param_1 + 0x38),0x14,local_6,local_4);
              FUN_1008_31d4(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0xc)
              ;
              uVar4 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + local_4 * 4);
              *(undefined2 *)((int)uVar4 + local_6 * 2) = 0;
              local_1a = 1;
            }
            iVar6 = FUN_1008_91ec((int)param_1,param_1._2_2_,local_6,local_4,local_c);
            if ((iVar6 != 0) &&
               ((local_8 = FUN_1008_93a4((int)param_1,param_1._2_2_,local_1e,unaff_SS,local_6,
                                         local_4), local_8 < local_e ||
                ((local_8 == local_e && (iVar6 = FUN_1008_3c88(100), 0x46 < iVar6)))))) {
              local_16 = local_a;
              local_e = local_8;
              local_14 = iVar7;
            }
          }
        }
        iVar6 = local_a + 1;
      }
LAB_1008_864d:
      if (local_1a == 0) {
        FUN_1008_a060((int)param_1,param_1._2_2_,local_12,local_10);
        pbVar1 = (byte *)((local_10 * 0x24 + local_12) * 2);
        *pbVar1 = *pbVar1 & 0xfd;
        if (*(int *)(local_c * 0xc + (int)*(undefined4 *)((int)param_1 + 0x2c) + 10) != 1) {
          piVar2 = (int *)(local_c * 0xc + (int)*(undefined4 *)((int)param_1 + 0x2c) + 2);
          *piVar2 = *piVar2 + local_16;
          piVar2 = (int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + local_c * 0xc + 4);
          *piVar2 = *piVar2 + local_14;
        }
        uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10);
        iVar7 = (int)*(undefined4 *)((int)param_1 + 0x2c) + local_c * 0xc;
        local_a = *(int *)(iVar7 + 2);
        iVar7 = *(int *)(iVar7 + 4);
        FUN_1008_a060((int)param_1,param_1._2_2_,iVar7,local_a);
        pbVar1 = (byte *)((local_a * 0x24 + iVar7) * 2);
        *pbVar1 = *pbVar1 | 2;
        if ((*(int *)((int)*(undefined4 *)((int)param_1 + 4) + 0xea) == local_a) &&
           (*(int *)((int)*(undefined4 *)((int)param_1 + 4) + 0xec) == iVar7)) {
          FUN_1008_2920(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),
                        *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 0x2c) + local_c * 0xc));
          FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),
                        0x18,iVar7,local_a);
          FUN_1008_8032((int)param_1,param_1._2_2_,local_c);
        }
        else {
          local_6 = 0;
          local_8 = iVar7;
          for (iVar6 = 0; iVar5 = local_8, iVar7 = local_a, iVar6 < *(int *)((int)param_1 + 0x52);
              iVar6 = iVar6 + 1) {
            if ((*(int *)(iVar6 * 10 + (int)*(undefined4 *)((int)param_1 + 0x32) + 2) == local_a) &&
               (*(int *)(iVar6 * 10 + (int)*(undefined4 *)((int)param_1 + 0x32) + 4) == local_8)) {
              iVar3 = *(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + local_c * 0xc);
              local_4 = *(int *)((int)*(undefined4 *)((int)param_1 + 0x32) + iVar6 * 10);
              if (local_4 == 0) {
                if (((iVar3 != 0) && (iVar3 != 1)) && (iVar3 != 4)) {
                  if ((iVar3 == 2) || (iVar3 == 3)) {
                    FUN_1008_7f8e((int)param_1,param_1._2_2_,iVar6);
                    iVar6 = iVar6 + -1;
                    goto LAB_1008_87f1;
                  }
                  goto LAB_1008_87d4;
                }
                if ((iVar3 == 0) || (iVar3 == 1)) {
                  local_e = iVar3;
                  local_6 = iVar6;
                  FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                *(undefined2 *)((int)param_1 + 0x38),0x16,local_8,local_a);
                  FUN_1008_31d4(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),
                                9);
                  uVar4 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar7 * 4);
                  if (*(int *)((int)uVar4 + local_8 * 2) == 0) {
                    if (local_e == 1) {
                      uVar4 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar7 * 4);
                      *(undefined2 *)((int)uVar4 + local_8 * 2) = 4;
                    }
                    else {
                      iVar6 = FUN_1008_ced2((int)param_1,param_1._2_2_);
                      if (iVar6 < 4) {
                        uVar8 = 5;
                      }
                      else {
                        uVar8 = 6;
                      }
                      uVar4 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar7 * 4);
                      *(undefined2 *)((int)uVar4 + local_8 * 2) = uVar8;
                    }
                  }
                }
                else {
                  local_e = iVar3;
                  local_6 = iVar6;
                  FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                *(undefined2 *)((int)param_1 + 0x38),0x17,local_8,local_a);
                  FUN_1008_31d4(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),
                                6);
                }
                FUN_1008_ad16((int)param_1,param_1._2_2_,local_e);
                FUN_1008_8032((int)param_1,param_1._2_2_,local_c);
                iVar6 = local_6;
LAB_1008_89c9:
                FUN_1008_7f8e((int)param_1,param_1._2_2_,iVar6);
              }
              else {
LAB_1008_87d4:
                if (local_4 == 1) {
                  if ((*(int *)((int)*(undefined4 *)((int)param_1 + 0x32) + iVar6 * 10 + 6) != 0) ||
                     (*(int *)((int)*(undefined4 *)((int)param_1 + 0x32) + iVar6 * 10 + 8) != 0)) {
                    local_e = iVar3;
                    FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                  *(undefined2 *)((int)param_1 + 0x38),0x14,local_8,local_a);
                    FUN_1008_31d4(*(undefined2 *)((int)param_1 + 4),
                                  *(undefined2 *)((int)param_1 + 6),0xc);
                    if ((local_e == 2) && (iVar7 = FUN_1008_3c88(10), iVar7 == 2)) {
                      FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                    *(undefined2 *)((int)param_1 + 0x38),1,local_8,local_a);
                    }
                    if (local_e == 3) {
                      FUN_1008_bc2c((int)param_1,param_1._2_2_,local_8,local_a);
                    }
                    FUN_1008_ad16((int)param_1,param_1._2_2_,local_e);
                    FUN_1008_8032((int)param_1,param_1._2_2_,local_c);
                    goto LAB_1008_89c9;
                  }
                  local_e = iVar3;
                  FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                *(undefined2 *)((int)param_1 + 0x38),0x24,local_8,local_a);
                  FUN_1008_7f8e((int)param_1,param_1._2_2_,iVar6);
                  break;
                }
                if (local_4 == 2) {
                  local_e = iVar3;
                  local_6 = iVar6;
                  FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                *(undefined2 *)((int)param_1 + 0x38),0x14,local_8,local_a);
                  FUN_1008_31d4(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),
                                0xc);
                  if ((local_e == 2) && (iVar7 = FUN_1008_3c88(10), iVar7 == 2)) {
                    FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                  *(undefined2 *)((int)param_1 + 0x38),1,iVar5,local_a);
                  }
                  if (local_e == 3) {
                    FUN_1008_bc2c((int)param_1,param_1._2_2_,iVar5,local_a);
                  }
                  FUN_1008_ad16((int)param_1,param_1._2_2_,local_e);
                  FUN_1008_8032((int)param_1,param_1._2_2_,local_c);
                  FUN_1008_7f8e((int)param_1,param_1._2_2_,local_6);
                  FUN_1008_7f48((int)param_1,param_1._2_2_,iVar5,local_a);
                }
                else {
                  local_e = iVar3;
                  if (local_4 != 4) goto LAB_1008_87f1;
                  FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                *(undefined2 *)((int)param_1 + 0x38),0x14,local_8,local_a);
                  FUN_1008_31d4(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),
                                0xd);
                  if ((local_e == 2) && (iVar7 = FUN_1008_3c88(10), iVar7 == 2)) {
                    FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                  *(undefined2 *)((int)param_1 + 0x38),1,local_8,local_a);
                  }
                  if (local_e == 3) {
                    FUN_1008_bc2c((int)param_1,param_1._2_2_,local_8,local_a);
                  }
                  FUN_1008_ad16((int)param_1,param_1._2_2_,local_e);
                  FUN_1008_8032((int)param_1,param_1._2_2_,local_c);
                }
              }
              local_18 = 1;
              break;
            }
LAB_1008_87f1:
          }
          if (local_18 == 0) goto LAB_1008_8b09;
        }
        local_c = local_c + -1;
      }
    }
    else {
      if (*(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + iVar7 * 0xc + 10) != 3) {
        local_c = iVar7;
        if (*(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + iVar7 * 0xc + 10) == 2) {
          local_8 = 8;
          do {
            local_6 = FUN_1008_3c88(3);
            local_6 = local_6 + -2;
            local_4 = FUN_1008_3c88(3);
            local_4 = local_4 + -2;
            uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10);
            iVar6 = (int)*(undefined4 *)((int)param_1 + 0x2c) + iVar7 * 0xc;
            iVar6 = FUN_1008_91c6((int)param_1,param_1._2_2_,*(int *)(iVar6 + 4) + local_4,
                                  *(int *)(iVar6 + 2) + local_6);
            if ((iVar6 != 0) &&
               (uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10),
               iVar6 = (int)*(undefined4 *)((int)param_1 + 0x2c) + iVar7 * 0xc,
               iVar6 = FUN_1008_91ec((int)param_1,param_1._2_2_,*(int *)(iVar6 + 4) + local_4,
                                     *(int *)(iVar6 + 2) + local_6,iVar7), iVar6 != 0)) {
              local_16 = local_6;
              local_14 = local_4;
              break;
            }
            local_8 = local_8 + -1;
          } while (0 < local_8);
          if (local_8 < 1) {
            local_14 = 0;
            local_16 = 0;
          }
        }
        goto LAB_1008_864d;
      }
      uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10);
      iVar6 = (int)*(undefined4 *)((int)param_1 + 0x2c) + iVar7 * 0xc;
      local_4 = *(int *)(iVar6 + 6) + local_10;
      local_6 = *(int *)(iVar6 + 8) + local_12;
      iVar6 = FUN_1008_91c6((int)param_1,param_1._2_2_,local_6,local_4);
      if ((iVar6 != 0) &&
         (iVar6 = FUN_1008_91ec((int)param_1,param_1._2_2_,local_6,local_4,iVar7), iVar6 != 0)) {
        uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10);
        iVar6 = (int)*(undefined4 *)((int)param_1 + 0x2c) + iVar7 * 0xc;
        local_16 = *(int *)(iVar6 + 6);
        local_14 = *(int *)(iVar6 + 8);
        local_c = iVar7;
        goto LAB_1008_864d;
      }
      *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 0x2c) + iVar7 * 0xc + 10) = 0;
      local_c = iVar7;
    }
LAB_1008_8b09:
    iVar7 = local_c + 1;
    local_c = iVar7;
  } while( true );
}



/* ============================================================
   1008:8b2c  FUN_1008_8b2c
   ============================================================ */

void __stdcall16far FUN_1008_8b2c(undefined4 param_1,undefined2 param_2)

{
  int *piVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 unaff_CS;
  char *pcVar8;
  char *pcVar9;
  undefined2 unaff_SS;
  undefined1 local_14 [4];
  int local_10;
  undefined2 local_8;
  int local_4;
  
  bVar2 = false;
  uVar6 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  uVar7 = (undefined2)((ulong)*(undefined4 *)(iVar4 + 0x36) >> 0x10);
  iVar5 = (int)*(undefined4 *)(iVar4 + 0x36);
  local_4 = *(int *)(iVar5 + 4);
  local_8 = 0;
  iVar5 = *(int *)(iVar5 + 2);
  do {
    if (local_4 == 0 && iVar5 == 0) {
LAB_1008_8b71:
      if (bVar2) {
        iVar5 = SETTIMER(unaff_CS,0,0,1,10);
        if (iVar5 == 0) {
          FUN_1008_2d02(*(undefined2 *)(iVar4 + 4),*(undefined2 *)(iVar4 + 6),0,0xd13a,
                        (char *)s_Lucky_Chip_1080_1004 + 4);
        }
        else {
          iVar5 = 1;
          pcVar8 = (char *)0x11d0;
          do {
            pcVar9 = (char *)0x11d0;
            iVar3 = PEEKMESSAGE(pcVar8,1,0x113,0x113,0,local_14,unaff_SS);
            if ((iVar3 != 0) && (local_10 == 10)) {
              pcVar9 = (char *)s_Lucky_Chip_1080_1004 + 4;
              FUN_1008_8be6(iVar4,uVar6,iVar5,param_2);
              iVar5 = iVar5 + 1;
            }
            pcVar8 = pcVar9;
          } while (iVar5 < 6);
          KILLTIMER(pcVar9,10);
        }
      }
      return;
    }
    if ((0x13 < *(int *)(iVar5 + 4)) && (*(int *)(iVar5 + 4) < 0x20)) {
      bVar2 = true;
      goto LAB_1008_8b71;
    }
    piVar1 = (int *)(iVar5 + 6);
    local_4 = *(int *)(iVar5 + 8);
    iVar5 = *piVar1;
  } while( true );
}



/* ============================================================
   1008:8be6  FUN_1008_8be6
   ============================================================ */

void __stdcall16far FUN_1008_8be6(undefined4 *param_1,int param_2,undefined2 param_3)

{
  undefined2 uVar1;
  undefined2 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  char *pcVar8;
  undefined2 unaff_SS;
  undefined1 local_22 [6];
  undefined2 local_1c;
  undefined2 local_1a;
  undefined2 local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  undefined2 local_c;
  uint local_a;
  undefined4 local_8;
  int local_4;
  
  local_1c = GETDC();
  pcVar8 = (char *)s_Lucky_Chip_1080_1004 + 4;
  FUN_1008_3b6c(local_22,unaff_SS,local_1c);
  local_4 = 0x4b;
  local_c = 0xdc;
  if (param_2 == 1) {
    local_16 = 0x5b;
    local_18 = 0x60;
    local_1a = 100;
    local_e = 0x3a;
    local_10 = 0x4d;
    local_12 = 0x4e;
    local_c = 0xdd;
    local_14 = 0x1ce;
  }
  else if (param_2 == 2) {
    local_16 = 0x5c;
    local_18 = 0x61;
    local_1a = 0x65;
    local_e = 0x3b;
    local_10 = 0x4f;
    local_12 = 0x50;
    local_c = 0xde;
    local_14 = 0x1cf;
  }
  else if (param_2 == 3) {
    local_16 = 0x5d;
    local_18 = 0x62;
    local_1a = 0x66;
    local_e = 0x3c;
    local_10 = 0x51;
    local_12 = 0x52;
    local_c = 0xdf;
    local_14 = 0x1d0;
  }
  else if (param_2 == 4) {
    local_16 = 0x5e;
    local_18 = 99;
    local_1a = 0x67;
    local_e = 0x3d;
    local_10 = 0x53;
    local_12 = 0x54;
    local_c = 0xe0;
    local_14 = 0x1d1;
  }
  else if (param_2 == 5) {
    local_16 = 0x5f;
    local_18 = 0;
    local_1a = 0x68;
    local_e = 0x3e;
    local_10 = 0x55;
    local_12 = 0x56;
    local_c = 0xe1;
    local_14 = 0x1d2;
  }
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  puVar3 = (undefined4 *)param_1;
  uVar6 = (undefined2)((ulong)*(undefined4 *)((int)puVar3 + 0x36) >> 0x10);
  iVar4 = (int)*(undefined4 *)((int)puVar3 + 0x36);
  local_8 = (undefined2 *)
            CONCAT22(*(undefined2 *)(iVar4 + 4),(undefined2 *)*(undefined2 *)(iVar4 + 2));
  do {
    if (local_8._2_2_ == 0 && (undefined2 *)local_8 == (undefined2 *)0x0) {
      RELEASEDC(pcVar8,local_1c);
      FUN_1008_3ba6(local_22,unaff_SS);
      return;
    }
    local_a = 0;
    local_4 = ((undefined2 *)local_8)[2];
    uVar6 = *local_8;
    uVar1 = ((undefined2 *)local_8)[1];
    if ((local_4 < 0x14) || (0x1f < local_4)) goto switchD_1008_8d49_caseD_15;
    local_a = (uint)(4 < param_2);
    pcVar8 = (char *)s_Lucky_Chip_1080_1004 + 4;
    switch(local_4) {
    case 0x14:
      uVar7 = local_18;
      break;
    default:
      goto switchD_1008_8d49_caseD_15;
    case 0x16:
      uVar7 = local_16;
      break;
    case 0x17:
      uVar7 = local_1a;
      break;
    case 0x18:
      uVar7 = local_e;
      break;
    case 0x19:
      uVar7 = local_10;
      break;
    case 0x1a:
      uVar7 = local_12;
      break;
    case 0x1b:
      uVar7 = 0x4b;
      break;
    case 0x1c:
      uVar7 = 0x4c;
      break;
    case 0x1d:
      uVar7 = local_c;
      break;
    case 0x1e:
      puVar2 = (undefined2 *)((int)*param_1 + 8);
      iVar4 = (*(code *)*puVar2)((char *)s_Lucky_Chip_1080_1004 + 4,puVar3,uVar5,param_3);
      if (iVar4 == 5) {
        pcVar8 = (char *)0x1010;
        FUN_1010_2cac(puVar3,uVar5,param_2,uVar1,uVar6,local_22,unaff_SS);
      }
      goto switchD_1008_8d49_caseD_15;
    case 0x1f:
      uVar7 = local_14;
    }
    pcVar8 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_3bd0(local_22,unaff_SS,uVar1,uVar6,uVar7);
switchD_1008_8d49_caseD_15:
    uVar7 = (undefined2)((ulong)local_8 >> 0x10);
    local_8 = (undefined2 *)
              CONCAT22(((undefined2 *)local_8)[4],(undefined2 *)((undefined2 *)local_8)[3]);
    if (local_a != 0) {
      FUN_1008_afb0(*(undefined2 *)((int)puVar3 + 0x36),*(undefined2 *)(puVar3 + 0xe),local_4,uVar1,
                    uVar6);
      pcVar8 = (char *)s_Lucky_Chip_1080_1004 + 4;
      FUN_1008_a060(puVar3,uVar5,uVar1,uVar6);
    }
  } while( true );
}



/* ============================================================
   1008:8e22  FUN_1008_8e22
   ============================================================ */

void __stdcall16far
FUN_1008_8e22(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6,undefined2 param_7)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x52);
  if (iVar1 < 0x96) {
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x32) + iVar1 * 10) = param_7;
    *(undefined2 *)(iVar1 * 10 + (int)*(undefined4 *)(param_1 + 0x32) + 2) = param_6;
    *(undefined2 *)(iVar1 * 10 + (int)*(undefined4 *)(param_1 + 0x32) + 4) = param_5;
    *(undefined2 *)(iVar1 * 10 + (int)*(undefined4 *)(param_1 + 0x32) + 6) = param_4;
    *(undefined2 *)(iVar1 * 10 + (int)*(undefined4 *)(param_1 + 0x32) + 8) = param_3;
    iVar1 = iVar1 + 1;
  }
  *(int *)(param_1 + 0x52) = iVar1;
  return;
}



/* ============================================================
   1008:8ea4  FUN_1008_8ea4
   ============================================================ */

void __stdcall16far
FUN_1008_8ea4(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  int iVar2;
  undefined2 uVar3;
  undefined2 local_4;
  
  for (iVar2 = 0; iVar2 < *(int *)(param_1 + 0x50); iVar2 = iVar2 + 1) {
    iVar1 = *(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar2 * 0xc);
    if (iVar1 == 0) {
      local_4 = FUN_1008_ced2(param_1,param_2);
    }
    else if (iVar1 == 1) {
      local_4 = FUN_1008_cf26(param_1,param_2);
    }
    else if (iVar1 == 2) {
      local_4 = 7;
    }
    else if (iVar1 == 3) {
      local_4 = 0x10;
    }
    else if (iVar1 == 4) {
      local_4 = FUN_1008_cefc(param_1,param_2);
    }
    uVar3 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x2c) >> 0x10);
    iVar1 = (int)*(undefined4 *)(param_1 + 0x2c) + iVar2 * 0xc;
    FUN_1008_3bd0(param_3,param_4,*(undefined2 *)(iVar1 + 4),*(undefined2 *)(iVar1 + 2),local_4);
  }
  return;
}



/* ============================================================
   1008:8f32  FUN_1008_8f32
   ============================================================ */

void __stdcall16far
FUN_1008_8f32(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  
  uVar5 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar2 = (int)*(undefined4 *)(param_1 + 4);
  iVar1 = *(int *)(iVar2 + 0xea);
  iVar2 = *(int *)(iVar2 + 0xec);
  iVar4 = 0;
  do {
    if (*(int *)(param_1 + 0x52) <= iVar4) {
      return;
    }
    if ((*(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar4 * 10 + 2) != iVar1) ||
       (*(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar4 * 10 + 4) != iVar2)) {
      iVar3 = *(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar4 * 10);
      if (iVar3 == 0) {
        uVar5 = 0x12;
LAB_1008_9004:
        uVar6 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x32) >> 0x10);
        iVar3 = (int)*(undefined4 *)(param_1 + 0x32) + iVar4 * 10;
      }
      else {
        if (iVar3 == 1) {
          if ((*(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar4 * 10 + 6) == 0) &&
             (*(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar4 * 10 + 8) == 0)) {
            uVar5 = 0x14;
          }
          else {
            uVar5 = 0x13;
          }
          goto LAB_1008_9004;
        }
        if (iVar3 != 2) {
          if (iVar3 == 3) {
            uVar5 = 0x1d;
          }
          else {
            if (iVar3 != 4) goto LAB_1008_9023;
            uVar5 = 0x21;
          }
          goto LAB_1008_9004;
        }
        uVar6 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x32) >> 0x10);
        iVar3 = (int)*(undefined4 *)(param_1 + 0x32) + iVar4 * 10;
        uVar5 = FUN_1008_9036(param_1,param_2,*(undefined2 *)(iVar3 + 8),*(undefined2 *)(iVar3 + 6))
        ;
      }
      FUN_1008_3bd0(param_3,param_4,*(undefined2 *)(iVar3 + 4),*(undefined2 *)(iVar3 + 2),uVar5);
    }
LAB_1008_9023:
    iVar4 = iVar4 + 1;
  } while( true );
}



/* ============================================================
   1008:9036  FUN_1008_9036
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_9036(undefined2 param_1,undefined2 param_2,int param_3,int param_4)

{
  int iVar1;
  
  iVar1 = 0;
  while( true ) {
    if (7 < iVar1) {
      return 0x13;
    }
    if ((((undefined2 *)&DAT_1080_1072)[iVar1 * 3] == param_4) &&
       (((undefined2 *)&DAT_1080_1074)[iVar1 * 3] == param_3)) break;
    iVar1 = iVar1 + 1;
  }
  return *(undefined2 *)(iVar1 * 6 + 0x1076);
}



/* ============================================================
   1008:9070  FUN_1008_9070
   ============================================================ */

void __stdcall16far FUN_1008_9070(undefined4 *param_1,undefined2 param_2,undefined2 param_3)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined2 uVar6;
  
  FUN_1008_a652((undefined4 *)param_1,param_1._2_2_);
  uVar6 = (undefined2)((ulong)((undefined4 *)param_1)[1] >> 0x10);
  iVar3 = (int)((undefined4 *)param_1)[1];
  iVar1 = *(int *)(iVar3 + 0xea);
  iVar3 = *(int *)(iVar3 + 0xec);
  for (iVar5 = 0; iVar5 < 0x28; iVar5 = iVar5 + 1) {
    for (iVar4 = 0; iVar4 < 0x24; iVar4 = iVar4 + 1) {
      if ((((*(byte *)((iVar5 * 0x24 + iVar4) * 2) & 1) != 0) &&
          ((*(byte *)((iVar5 * 0x24 + iVar4) * 2) & 6) == 0)) &&
         ((iVar1 != iVar5 || (iVar3 != iVar4)))) {
        uVar2 = *(undefined4 *)((int)((undefined4 *)param_1)[2] + iVar5 * 4);
        if (*(int *)((int)uVar2 + iVar4 * 2) == 2) {
          (*(code *)*(undefined2 *)(undefined2 *)*param_1)
                    ((char *)s_Lucky_Chip_1080_1004 + 4,(undefined4 *)param_1,param_1._2_2_,param_2,
                     param_3,iVar4,iVar5);
        }
        else {
          uVar2 = *(undefined4 *)((int)((undefined4 *)param_1)[2] + iVar5 * 4);
          FUN_1008_3bd0(param_2,param_3,iVar4,iVar5,
                        *(undefined2 *)(*(int *)((int)uVar2 + iVar4 * 2) * 6 + 0x10a4));
        }
      }
    }
  }
  FUN_1008_b09e(*(undefined2 *)((int)(undefined4 *)param_1 + 0x36),
                *(undefined2 *)((undefined4 *)param_1 + 0xe),(undefined4 *)param_1,param_1._2_2_,
                *(undefined2 *)((undefined4 *)param_1 + 1),
                *(undefined2 *)((int)(undefined4 *)param_1 + 6),param_2,param_3);
  return;
}



/* ============================================================
   1008:916e  FUN_1008_916e
   ============================================================ */

undefined2 __stdcall16far FUN_1008_916e(undefined2 param_1,undefined2 param_2,int param_3)

{
  return *(undefined2 *)(param_3 * 6 + 0x10a6);
}



/* ============================================================
   1008:917e  FUN_1008_917e
   ============================================================ */

bool __stdcall16far FUN_1008_917e(undefined4 param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  bool bVar4;
  
  iVar2 = FUN_1008_91c6((int)param_1,param_1._2_2_,param_2,param_3);
  bVar4 = false;
  if (iVar2 != 0) {
    uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4);
    uVar3 = FUN_1008_916e((int)param_1,param_1._2_2_,*(undefined2 *)((int)uVar1 + param_2 * 2));
    bVar4 = (uVar3 & 1) != 0;
  }
  return bVar4;
}



/* ============================================================
   1008:91c6  FUN_1008_91c6
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_91c6(undefined2 param_1,undefined2 param_2,int param_3,int param_4)

{
  undefined2 uVar1;
  
  if ((((param_4 < 0) || (0x27 < param_4)) || (param_3 < 0)) || (0x23 < param_3)) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



/* ============================================================
   1008:91ec  FUN_1008_91ec
   ============================================================ */

undefined2 __stdcall16far FUN_1008_91ec(undefined4 param_1,int param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4);
  if ((*(int *)((int)uVar1 + param_2 * 2) == 0) &&
     ((*(byte *)((param_3 * 0x24 + param_2) * 2) & 2) == 0)) {
    if (*(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + param_4 * 0xc) == 0) {
      return 1;
    }
    if (*(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + param_4 * 0xc) == 4) {
      return 1;
    }
    if (*(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + param_4 * 0xc) == 1) {
      return 1;
    }
    if ((*(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + param_4 * 0xc) == 2) ||
       (*(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + param_4 * 0xc) == 3)) {
      if ((*(byte *)((param_3 * 0x24 + param_2) * 2) & 4) == 0) {
        return 1;
      }
      iVar2 = FUN_1008_3c88(2);
      if (iVar2 == 2) {
        return 0;
      }
      return 1;
    }
  }
  uVar3 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 4) >> 0x10);
  iVar2 = (int)*(undefined4 *)((int)param_1 + 4);
  iVar2 = FUN_1008_917e(*(undefined2 *)(iVar2 + 0xdc),*(undefined2 *)(iVar2 + 0xde),param_2,param_3)
  ;
  if ((((iVar2 != 0) ||
       ((uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4),
        7 < *(int *)((int)uVar1 + param_2 * 2) &&
        (uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4),
        *(int *)((int)uVar1 + param_2 * 2) < 0x18)))) ||
      ((uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4),
       0x86 < *(int *)((int)uVar1 + param_2 * 2) &&
       (uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4),
       *(int *)((int)uVar1 + param_2 * 2) < 0xa2)))) &&
     (((*(byte *)((param_3 * 0x24 + param_2) * 2) & 2) == 0 &&
      (((uVar3 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10),
        *(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + param_4 * 0xc) != 2 &&
        (*(int *)(*(int *)((int)param_1 + 0x2c) + param_4 * 0xc) != 3)) ||
       ((*(byte *)((param_3 * 0x24 + param_2) * 2) & 4) == 0)))))) {
    return 1;
  }
  return 0;
}



/* ============================================================
   1008:93a4  FUN_1008_93a4
   ============================================================ */

int __stdcall16far
FUN_1008_93a4(int param_1,undefined2 param_2,undefined2 *param_3,int param_4,int param_5)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  undefined2 uVar6;
  int local_6;
  
  if (*(int *)((int)*(undefined4 *)(param_1 + 4) + 0xfa) == 0) {
    uVar6 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
    iVar5 = (int)*(undefined4 *)(param_1 + 4);
    iVar2 = FUN_1000_1f42(*(int *)(iVar5 + 0xec) - param_4);
    iVar5 = FUN_1000_1f42(*(int *)(iVar5 + 0xea) - param_5);
    if (iVar2 < iVar5) {
      iVar2 = *(int *)((int)*(undefined4 *)(param_1 + 4) + 0xea) - param_5;
    }
    else {
      iVar2 = *(int *)((int)*(undefined4 *)(param_1 + 4) + 0xec) - param_4;
    }
    local_6 = FUN_1000_1f42(iVar2);
  }
  else {
    uVar6 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
    iVar5 = (int)*(undefined4 *)(param_1 + 4);
    iVar2 = FUN_1000_1f42(*(int *)(iVar5 + 0xea) - param_5);
    iVar5 = FUN_1000_1f42(*(int *)(iVar5 + 0xec) - param_4);
    if (iVar5 < iVar2) {
      iVar2 = *(int *)((int)*(undefined4 *)(param_1 + 4) + 0xea) - param_5;
    }
    else {
      iVar2 = *(int *)((int)*(undefined4 *)(param_1 + 4) + 0xec) - param_4;
    }
    iVar2 = FUN_1000_1f42(iVar2);
    local_6 = FUN_1000_1f42(-(iVar2 + -5));
  }
  iVar2 = ((undefined2 *)param_3)[1];
  piVar3 = (int *)*param_3;
  while (iVar2 != 0 || piVar3 != (int *)0x0) {
    iVar5 = FUN_1000_1f42(piVar3[1] - param_4);
    iVar4 = FUN_1000_1f42(*piVar3 - param_5);
    if (iVar5 < iVar4) {
      iVar5 = *piVar3 - param_5;
    }
    else {
      iVar5 = piVar3[1] - param_4;
    }
    iVar5 = FUN_1000_1f42(iVar5);
    if (iVar5 < local_6) {
      local_6 = iVar5;
    }
    piVar1 = piVar3 + 2;
    iVar2 = piVar3[3];
    piVar3 = (int *)*piVar1;
  }
  return local_6;
}



/* ============================================================
   1008:9504  FUN_1008_9504
   ============================================================ */

void __stdcall16far
FUN_1008_9504(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  for (iVar3 = 0; iVar3 < param_5; iVar3 = iVar3 + 1) {
    iVar1 = FUN_1008_3c88(0x28);
    iVar1 = iVar1 + -1;
    iVar2 = FUN_1008_3c88(0x24,iVar1);
    FUN_1008_9594(param_1,param_2,param_3,param_4,iVar2 + -1,iVar1);
  }
  return;
}



/* ============================================================
   1008:9540  FUN_1008_9540
   ============================================================ */

void __stdcall16far FUN_1008_9540(undefined4 param_1,undefined2 param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = FUN_1008_91c6((int)param_1,param_1._2_2_,param_3,param_4);
  if ((iVar2 != 0) &&
     (uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_4 * 4),
     *(int *)((int)uVar1 + param_3 * 2) == 0)) {
    uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_4 * 4);
    *(undefined2 *)((int)uVar1 + param_3 * 2) = param_2;
  }
  return;
}



/* ============================================================
   1008:9594  FUN_1008_9594
   ============================================================ */

void __stdcall16far
FUN_1008_9594(undefined2 param_1,undefined2 param_2,int param_3,undefined2 param_4,int param_5,
             int param_6)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_6;
  
  iVar1 = FUN_1008_3c88(param_4);
  iVar2 = FUN_1008_3c88(param_4);
  iVar3 = FUN_1008_3c88(4);
  iVar3 = iVar3 + -2;
  local_a = FUN_1008_3c88(4);
  local_a = local_a + -2;
  local_8 = -((iVar1 + 1) - param_6);
  do {
    if (iVar2 + 1 + param_6 < local_8) {
      return;
    }
    for (iVar1 = param_5 - iVar3; iVar1 <= param_5 + local_a; iVar1 = iVar1 + 1) {
      if (param_3 == 0xaf) {
        iVar4 = FUN_1008_3c88(0xc);
        if (iVar4 != 1) goto LAB_1008_9602;
        local_6 = 0xaf;
      }
      else {
LAB_1008_9602:
        local_6 = 3;
      }
      FUN_1008_9540(param_1,param_2,local_6,iVar1,local_8);
    }
    if (local_8 < param_6) {
      iVar1 = FUN_1008_3c88(4);
      iVar1 = iVar1 + -2;
      iVar4 = FUN_1008_3c88(4);
      iVar4 = iVar4 + -2;
    }
    else {
      iVar1 = FUN_1008_3c88(4);
      iVar4 = -(iVar1 + -1);
      iVar1 = FUN_1008_3c88(4);
      iVar1 = -(iVar1 + -1);
    }
    iVar3 = iVar3 + iVar1;
    local_a = local_a + iVar4;
    local_8 = local_8 + 1;
  } while( true );
}



/* ============================================================
   1008:9680  FUN_1008_9680
   ============================================================ */

void __stdcall16far FUN_1008_9680(undefined4 *param_1,undefined2 param_2)

{
  undefined2 *puVar1;
  undefined4 uVar2;
  undefined2 uVar3;
  int iVar4;
  char *pcVar5;
  undefined2 unaff_SS;
  undefined1 local_8 [6];
  
  FUN_1008_a652(param_1,param_2);
  uVar2 = *(undefined4 *)((int)param_1[1] + 0xc);
  uVar3 = GETDC((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)((int)uVar2 + 8));
  FUN_1008_3b6c(local_8,unaff_SS,uVar3);
  FUN_1008_9786(param_1,param_2,local_8,unaff_SS);
  FUN_1008_9c5a(param_1,param_2,local_8,unaff_SS);
  FUN_1008_9d9c(param_1,param_2,local_8,unaff_SS);
  FUN_1008_9e68(param_1,param_2,local_8,unaff_SS);
  pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
  FUN_1008_974c(param_1,param_2,local_8,unaff_SS);
  puVar1 = (undefined2 *)((int)*param_1 + 8);
  iVar4 = (*(code *)*puVar1)((char *)s_Lucky_Chip_1080_1004 + 4,param_1,param_2);
  if (iVar4 == 5) {
    pcVar5 = (char *)0x1010;
    FUN_1010_26fe(param_1,param_2,uVar3);
  }
  DAT_1080_14ce = DAT_1080_14ce + 1;
  if (DAT_1080_14ce == 0x78) {
    DAT_1080_14ce = 0;
  }
  RELEASEDC(pcVar5,uVar3);
  FUN_1008_3ba6(local_8,unaff_SS);
  return;
}



/* ============================================================
   1008:974c  FUN_1008_974c
   ============================================================ */

void __stdcall16far
FUN_1008_974c(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  undefined2 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar1 = FUN_1008_2d50(*(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6));
  uVar3 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar2 = (int)*(undefined4 *)(param_1 + 4);
  FUN_1008_3bd0(param_3,param_4,*(undefined2 *)(iVar2 + 0xec),*(undefined2 *)(iVar2 + 0xea),uVar1);
  return;
}



/* ============================================================
   1008:9786  FUN_1008_9786
   ============================================================ */

int __stdcall16far FUN_1008_9786(undefined4 param_1,undefined2 param_2,undefined2 param_3)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined2 uVar8;
  int iVar9;
  
  uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 4) >> 0x10);
  iVar5 = (int)*(undefined4 *)((int)param_1 + 4);
  iVar1 = *(int *)(iVar5 + 0xea);
  iVar5 = *(int *)(iVar5 + 0xec);
  iVar7 = 0;
  iVar3 = iVar5;
  do {
    for (iVar6 = 0; iVar6 < 0x24; iVar6 = iVar6 + 1) {
      iVar3 = iVar6 * 2;
      uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar7 * 4);
      if (*(int *)((int)uVar2 + iVar3) == 0) goto LAB_1008_9c42;
      uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar7 * 4);
      if (((2 < *(int *)((int)uVar2 + iVar6 * 2)) &&
          (uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar7 * 4),
          *(int *)((int)uVar2 + iVar6 * 2) < 8)) ||
         (iVar3 = iVar6 * 2, (*(byte *)((iVar7 * 0x24 + iVar6) * 2) & 6) != 0)) {
        iVar3 = iVar6 * 2;
        goto LAB_1008_9c42;
      }
      if ((iVar1 == iVar7) && (iVar5 == iVar6)) goto LAB_1008_9c42;
      iVar3 = iVar6 * 2;
      uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar7 * 4);
      iVar9 = *(int *)((int)uVar2 + iVar3);
      iVar4 = (int)*(undefined4 *)((int)param_1 + 4);
      if (*(int *)(iVar4 + 0xfe) != 0) {
switchD_1008_9863_caseD_9:
        if (iVar9 == 0x49) {
          if ((int)DAT_1080_14ce % 10 == 1) {
            iVar9 = 0x70;
          }
          else {
            iVar3 = (int)DAT_1080_14ce / 10;
            if ((int)DAT_1080_14ce % 10 != 5) goto LAB_1008_9c42;
            iVar9 = 0x71;
          }
        }
        else if ((iVar9 < 0x27) || (0x2a < iVar9)) {
          if ((iVar9 < 0x23) || (0x26 < iVar9)) {
            if ((iVar9 < 0x51) ||
               (iVar3 = iVar6 * 2,
               uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar7 * 4),
               0x54 < *(int *)((int)uVar2 + iVar3))) {
              if (iVar9 == 0x43) {
                iVar3 = (int)DAT_1080_14ce % 8;
                switch(iVar3) {
                case 0:
switchD_1008_99fc_caseD_0:
                  iVar9 = 0xa8;
                  break;
                case 1:
                case 7:
switchD_1008_99fc_caseD_1:
                  iVar9 = 0xaa;
                  break;
                case 2:
                case 6:
switchD_1008_99fc_caseD_2:
                  iVar9 = 0xa9;
                  break;
                case 3:
                case 5:
switchD_1008_99fc_caseD_3:
                  iVar9 = 0xab;
                  break;
                case 4:
switchD_1008_99fc_caseD_4:
                  iVar9 = 0xa7;
                  break;
                default:
                  goto switchD_1008_99fc_default;
                }
              }
              else {
switchD_1008_99fc_default:
                if (iVar9 == 0x44) {
                  iVar3 = (int)DAT_1080_14ce % 8;
                  switch(iVar3) {
                  case 0:
                    goto switchD_1008_99fc_caseD_0;
                  case 1:
                  case 7:
switchD_1008_9a30_caseD_1:
                    iVar9 = 0xac;
                    break;
                  case 2:
                  case 6:
switchD_1008_9a30_caseD_2:
                    iVar9 = 0xad;
                    break;
                  case 3:
                  case 5:
switchD_1008_9a30_caseD_3:
                    iVar9 = 0xae;
                    break;
                  case 4:
                    goto switchD_1008_99fc_caseD_4;
                  default:
                    goto switchD_1008_9a30_default;
                  }
                }
                else {
switchD_1008_9a30_default:
                  if (iVar9 == 0x41) {
                    iVar3 = (int)DAT_1080_14ce % 8;
                    switch(iVar3) {
                    case 0:
                      goto switchD_1008_99fc_caseD_4;
                    case 1:
                    case 7:
                      goto switchD_1008_99fc_caseD_3;
                    case 2:
                    case 6:
                      goto switchD_1008_99fc_caseD_2;
                    case 3:
                    case 5:
                      goto switchD_1008_99fc_caseD_1;
                    case 4:
                      goto switchD_1008_99fc_caseD_0;
                    }
                  }
                  if (iVar9 == 0x42) {
                    iVar3 = (int)DAT_1080_14ce % 8;
                    switch(iVar3) {
                    case 0:
                      goto switchD_1008_99fc_caseD_4;
                    case 1:
                    case 7:
                      goto switchD_1008_9a30_caseD_3;
                    case 2:
                    case 6:
                      goto switchD_1008_9a30_caseD_2;
                    case 3:
                    case 5:
                      goto switchD_1008_9a30_caseD_1;
                    case 4:
                      goto switchD_1008_99fc_caseD_0;
                    }
                  }
                  if (iVar9 == 0xab) {
                    if ((int)DAT_1080_14ce % 4 == 0) {
                      iVar9 = 0x1c5;
                    }
                    else {
                      iVar3 = (int)DAT_1080_14ce % 4 + -1;
                      if (iVar3 != 0) goto LAB_1008_9c42;
                      iVar9 = 0x1c6;
                    }
                  }
                  else if (iVar9 == 0xad) {
                    iVar3 = (int)DAT_1080_14ce % 3;
                    if (iVar3 == 0) {
                      iVar9 = 0x1c8;
                    }
                    else if (iVar3 == 1) {
                      iVar9 = 0x1c9;
                    }
                    else {
                      iVar3 = iVar3 + -2;
                      if (iVar3 != 0) goto LAB_1008_9c42;
                      iVar9 = 0x1ca;
                    }
                  }
                  else {
                    if ((iVar9 < 0x83) || (0x86 < iVar9)) {
                      if ((iVar9 < 0xa2) || (0xaa < iVar9)) {
                        if (iVar9 == 0x59) {
                          iVar3 = (int)DAT_1080_14ce / 10;
                          if ((int)DAT_1080_14ce % 10 == 0) {
                            iVar9 = ((int)DAT_1080_14ce % 0x28) / 10 + 0xd3;
                            goto LAB_1008_9c35;
                          }
                        }
                        else if (iVar9 == 0x5b) {
                          if ((DAT_1080_14ce & 1) == 0) {
                            iVar9 = ((int)DAT_1080_14ce % 8) / 2 + 0xd8;
                            goto LAB_1008_9c35;
                          }
                        }
                        else if (iVar9 == 0xaf) {
                          if ((DAT_1080_14ce & 1) == 0) {
                            iVar9 = FUN_1008_3c88(2);
                            iVar9 = iVar9 + 0x1d2;
                            goto LAB_1008_9c35;
                          }
                        }
                        else if ((iVar9 == 0xb0) && ((DAT_1080_14ce & 1) == 0)) {
                          iVar9 = ((int)DAT_1080_14ce % 6) / 2 + 0x1d5;
                          goto LAB_1008_9c35;
                        }
                      }
                      else {
                        iVar3 = FUN_1008_3c88(3);
                        if (iVar3 < 2) {
                          iVar3 = FUN_1008_3c88(2);
                          if (iVar3 == 2) {
                            iVar3 = iVar9 + -0xa2;
                            if (iVar3 < 2) {
                              iVar9 = iVar9 + 0xa2;
                            }
                            else if ((iVar3 == 3) || (iVar3 == 4)) {
                              iVar9 = iVar9 + 0xa1;
                            }
                            else {
                              iVar9 = iVar9 + 0x99;
                            }
                          }
                          else {
                            iVar9 = iVar9 + 0xa6;
                          }
                          goto LAB_1008_9c35;
                        }
                      }
                      goto LAB_1008_9c42;
                    }
                    iVar3 = FUN_1008_3c88(8);
                    if (iVar3 == 1) {
                      iVar9 = iVar9 + 0x103;
                    }
                    else {
                      iVar9 = iVar9 + 0xff;
                    }
                  }
                }
              }
            }
            else {
              uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar7 * 4);
              iVar9 = ((int)DAT_1080_14ce % 9) * 4 + *(int *)((int)uVar2 + iVar6 * 2) + 0x5e;
            }
          }
          else if ((int)DAT_1080_14ce % 8 < 4) {
            iVar9 = (int)DAT_1080_14ce % 4 + 0x42;
          }
          else {
            iVar9 = 0x41;
          }
        }
        else if ((int)DAT_1080_14ce % 8 < 4) {
          iVar9 = (int)DAT_1080_14ce % 4 + 0x2c;
        }
        else {
          iVar9 = 0x2b;
        }
        goto LAB_1008_9c35;
      }
      iVar3 = iVar4;
      switch(iVar9 + -0x18) {
      case 0:
      case 1:
      case 6:
      case 7:
        if ((int)DAT_1080_14ce % 5 == 1) break;
        iVar3 = (int)DAT_1080_14ce / 5;
        if ((int)DAT_1080_14ce % 5 != 2) goto LAB_1008_9c42;
        iVar9 = 0x6e;
        goto LAB_1008_9c35;
      case 2:
      case 3:
        iVar3 = (int)DAT_1080_14ce % 3;
        if (iVar3 != 0) {
          if (iVar3 != 1) {
            if (iVar3 == 2) {
              iVar9 = 0x73;
              goto LAB_1008_9c35;
            }
            goto switchD_1008_9863_caseD_4;
          }
          goto LAB_1008_9905;
        }
        break;
      case 4:
      case 5:
switchD_1008_9863_caseD_4:
        iVar3 = (int)DAT_1080_14ce / 2;
        if ((int)DAT_1080_14ce % 2 != 1) {
          if ((DAT_1080_14ce & 1) != 0) goto LAB_1008_9c42;
LAB_1008_9905:
          iVar9 = 0x6f;
          goto LAB_1008_9c35;
        }
        break;
      case 8:
        iVar9 = 0x14;
        if ((int)DAT_1080_14ce % 0x14 == 10) {
LAB_1008_98f6:
          iVar9 = 0x2a;
          goto LAB_1008_9c35;
        }
        goto LAB_1008_98d6;
      default:
        goto switchD_1008_9863_caseD_9;
      case 10:
        iVar9 = 6;
        if ((int)DAT_1080_14ce % 6 == 3) goto LAB_1008_98f6;
LAB_1008_98d6:
        iVar3 = (int)DAT_1080_14ce / iVar9;
        if ((int)DAT_1080_14ce % iVar9 == 0) {
          iVar9 = 0x29;
          goto LAB_1008_9c35;
        }
        goto LAB_1008_9c42;
      }
      iVar9 = 0x6d;
LAB_1008_9c35:
      iVar3 = FUN_1008_3bd0(param_2,param_3,iVar6,iVar7,iVar9);
LAB_1008_9c42:
    }
    iVar7 = iVar7 + 1;
    if (0x27 < iVar7) {
      return iVar3;
    }
  } while( true );
}



/* ============================================================
   1008:9c5a  FUN_1008_9c5a
   ============================================================ */

void __stdcall16far FUN_1008_9c5a(undefined4 param_1,undefined2 param_2,undefined2 param_3)

{
  bool bVar1;
  char cVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  uint uVar6;
  int iVar7;
  undefined2 uVar8;
  undefined2 local_c;
  int local_4;
  
  uVar3 = FUN_1008_ced2((int)param_1,param_1._2_2_);
  uVar4 = FUN_1008_cefc((int)param_1,param_1._2_2_);
  uVar5 = FUN_1008_cf26((int)param_1,param_1._2_2_);
  local_4 = 0;
  do {
    if (*(int *)((int)param_1 + 0x50) <= local_4) {
      return;
    }
    bVar1 = false;
    iVar7 = *(int *)((int)*(undefined4 *)((int)param_1 + 0x2c) + local_4 * 0xc);
    uVar8 = uVar3;
    if ((iVar7 == 0) || (uVar8 = uVar5, iVar7 == 1)) {
LAB_1008_9d64:
      bVar1 = true;
      local_c = uVar8;
    }
    else {
      if (iVar7 != 2) {
        if (iVar7 == 3) {
          if (DAT_1080_14ce % 6 == 0) {
            uVar8 = 0x10;
          }
          else {
            if ((DAT_1080_14ce % 6 != 4) || (iVar7 = FUN_1008_3c88(2), iVar7 != 2))
            goto LAB_1008_9d67;
            uVar8 = 0x11;
          }
        }
        else {
          uVar8 = uVar4;
          if (iVar7 != 4) goto LAB_1008_9d67;
        }
        goto LAB_1008_9d64;
      }
      uVar6 = DAT_1080_14ce % 0xf;
      if (uVar6 == 0xb) goto LAB_1008_9d05;
      if (uVar6 < 0xc) {
        cVar2 = (char)uVar6;
        if (cVar2 == '\0') {
LAB_1008_9d15:
          uVar8 = 7;
          goto LAB_1008_9d64;
        }
        if (cVar2 == '\x04') {
          iVar7 = FUN_1008_3c88(2);
          if (iVar7 == 2) {
            uVar8 = 8;
            goto LAB_1008_9d64;
          }
        }
        else {
          if (cVar2 != '\x06') {
            if (cVar2 != '\t') goto LAB_1008_9d67;
            iVar7 = FUN_1008_3c88(2);
            if (iVar7 == 2) {
              uVar8 = 9;
              goto LAB_1008_9d64;
            }
          }
LAB_1008_9d05:
          iVar7 = FUN_1008_3c88(2);
          if (iVar7 == 2) goto LAB_1008_9d15;
        }
      }
    }
LAB_1008_9d67:
    if (bVar1) {
      uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10);
      iVar7 = (int)*(undefined4 *)((int)param_1 + 0x2c) + local_4 * 0xc;
      FUN_1008_3bd0(param_2,param_3,*(undefined2 *)(iVar7 + 4),*(undefined2 *)(iVar7 + 2),local_c);
    }
    local_4 = local_4 + 1;
  } while( true );
}



/* ============================================================
   1008:9d9c  FUN_1008_9d9c
   ============================================================ */

int __stdcall16far
FUN_1008_9d9c(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined2 uVar8;
  int local_e;
  
  uVar8 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar5 = (int)*(undefined4 *)(param_1 + 4);
  iVar1 = *(int *)(iVar5 + 0xea);
  iVar5 = *(int *)(iVar5 + 0xec);
  iVar7 = 0;
  iVar6 = iVar5;
  do {
    if (*(int *)(param_1 + 0x52) <= iVar7) {
      return iVar6;
    }
    uVar8 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x32) >> 0x10);
    iVar6 = (int)*(undefined4 *)(param_1 + 0x32) + iVar7 * 10;
    iVar2 = *(int *)(iVar6 + 2);
    iVar3 = *(int *)(iVar6 + 4);
    iVar6 = iVar2;
    if (((*(byte *)((iVar2 * 0x24 + iVar3) * 2) & 2) == 0) &&
       ((iVar2 != iVar1 || (iVar6 = iVar5, iVar3 != iVar5)))) {
      bVar4 = false;
      iVar6 = *(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar7 * 10);
      if (iVar6 == 3) {
        local_e = DAT_1080_14ce % 4 + 0x1d;
LAB_1008_9e32:
        iVar6 = DAT_1080_14ce / 4;
        bVar4 = true;
      }
      else {
        iVar6 = iVar6 + -4;
        if (iVar6 == 0) {
          local_e = DAT_1080_14ce % 4 + 0x21;
          goto LAB_1008_9e32;
        }
      }
      if (bVar4) {
        iVar6 = FUN_1008_3bd0(param_3,param_4,iVar3,iVar2,local_e);
      }
    }
    iVar7 = iVar7 + 1;
  } while( true );
}



/* ============================================================
   1008:9e68  FUN_1008_9e68
   ============================================================ */

void __stdcall16far
FUN_1008_9e68(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  bool bVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined2 uVar10;
  int local_e;
  
  uVar10 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar8 = (int)*(undefined4 *)(param_1 + 4);
  iVar2 = *(int *)(iVar8 + 0xea);
  iVar8 = *(int *)(iVar8 + 0xec);
  uVar10 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x36) >> 0x10);
  iVar9 = (int)*(undefined4 *)(param_1 + 0x36);
  iVar7 = *(int *)(iVar9 + 4);
  piVar5 = (int *)*(undefined2 *)(iVar9 + 2);
  do {
    if (iVar7 == 0 && piVar5 == (int *)0x0) {
      return;
    }
    bVar4 = false;
    iVar9 = *piVar5;
    iVar3 = piVar5[1];
    if (((*(byte *)((iVar3 + iVar9 * 0x24) * 2) & 6) != 0) ||
       (((iVar9 == iVar2 && (iVar3 == iVar8)) ||
        (iVar6 = FUN_1008_917e(param_1,param_2,iVar3,iVar9), iVar6 == 0)))) goto LAB_1008_a04b;
    switch(piVar5[2]) {
    case 1:
      if (DAT_1080_14ce % 6 == 0) {
        local_e = 0x46;
      }
      else if ((DAT_1080_14ce % 6 == 4) && (iVar6 = FUN_1008_3c88(3), iVar6 == 1)) {
        local_e = 0x47;
      }
      else {
        if (DAT_1080_14ce % 6 != 5) goto switchD_1008_9f09_caseD_4;
        iVar6 = FUN_1008_3c88(3);
        if (iVar6 != 1) break;
        local_e = 0x48;
      }
      goto LAB_1008_9f7e;
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xf:
    case 0x10:
    case 0x11:
      local_e = DAT_1080_14ce % 4 + 0x57;
LAB_1008_9f7e:
      bVar4 = true;
      break;
    default:
switchD_1008_9f09_caseD_4:
      break;
    case 0x2b:
      if (DAT_1080_14ce % 5 == 0) {
        bVar4 = true;
        local_e = 0xfb;
      }
      else {
        if (DAT_1080_14ce % 5 != 3) goto switchD_1008_9f09_caseD_4;
        bVar4 = true;
        local_e = 0;
      }
      break;
    case 0x2d:
      bVar4 = true;
      iVar6 = DAT_1080_14ce % 10;
      if (iVar6 == 1) {
LAB_1008_a020:
        local_e = 0xfd;
      }
      else if (iVar6 == 2) {
LAB_1008_a025:
        local_e = 0xfe;
      }
      else if (iVar6 == 3) {
        local_e = 0xff;
      }
      else {
        if (iVar6 == 4) goto LAB_1008_a025;
        if (iVar6 == 5) goto LAB_1008_a020;
        local_e = 0xfc;
      }
    }
    if (bVar4) {
      FUN_1008_3bd0(param_3,param_4,iVar3,iVar9,local_e);
    }
LAB_1008_a04b:
    piVar1 = piVar5 + 3;
    iVar7 = piVar5[4];
    piVar5 = (int *)*piVar1;
  } while( true );
}



/* ============================================================
   1008:a060  FUN_1008_a060
   ============================================================ */

void __stdcall16far FUN_1008_a060(undefined4 param_1,int param_2,int param_3)

{
  byte *pbVar1;
  undefined4 uVar2;
  int iVar3;
  undefined2 unaff_SS;
  undefined1 local_a [8];
  
  iVar3 = FUN_1008_91c6((int)param_1,param_1._2_2_,param_2,param_3);
  if (iVar3 != 0) {
    SETRECT((char *)s_Lucky_Chip_1080_1004 + 4,param_2 * 0xc + 0xc,param_3 * 0xc + 0xc,param_2 * 0xc
            ,param_3 * 0xc,local_a);
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xc);
    INVALIDATERECT(0x11d0,0,local_a,unaff_SS,*(undefined2 *)((int)uVar2 + 8));
    pbVar1 = (byte *)((param_2 + param_3 * 0x24) * 2);
    *pbVar1 = *pbVar1 | 1;
  }
  return;
}



/* ============================================================
   1008:a0ca  FUN_1008_a0ca
   ============================================================ */

void __stdcall16far FUN_1008_a0ca(undefined4 *param_1,undefined2 param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  undefined2 *puVar4;
  bool bVar5;
  int iVar6;
  int *piVar7;
  undefined2 uVar8;
  char *unaff_CS;
  int local_c;
  
  uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x36) >> 0x10);
  iVar6 = (int)*(undefined4 *)((int)param_1 + 0x36);
  local_c = *(int *)(iVar6 + 4);
  piVar7 = (int *)*(undefined2 *)(iVar6 + 2);
  while (local_c != 0 || piVar7 != (int *)0x0) {
    bVar5 = false;
    iVar6 = *piVar7;
    iVar1 = piVar7[1];
    iVar2 = piVar7[2];
    if ((((iVar6 == param_4) && (iVar1 == param_3)) && (0 < iVar2)) && (iVar2 < 0x12)) {
      puVar4 = (undefined2 *)((int)*param_1 + 0x10);
      (*(code *)*puVar4)(unaff_CS,param_1,param_2,iVar2);
      bVar5 = true;
    }
    piVar3 = (int *)piVar7[3];
    local_c = piVar7[4];
    piVar7 = piVar3;
    if (bVar5) {
      unaff_CS = (char *)s_Lucky_Chip_1080_1004 + 4;
      FUN_1008_afb0(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),iVar2,iVar1,
                    iVar6);
    }
  }
  return;
}



/* ============================================================
   1008:a5fa  FUN_1008_a5fa
   ============================================================ */

void __cdecl16far FUN_1008_a5fa(void)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  
  for (iVar3 = 0; iVar3 < 0x28; iVar3 = iVar3 + 1) {
    for (iVar2 = 0; iVar2 < 0x24; iVar2 = iVar2 + 1) {
      pbVar1 = (byte *)((iVar3 * 0x24 + iVar2) * 2);
      *pbVar1 = *pbVar1 | 1;
    }
  }
  return;
}



/* ============================================================
   1008:a626  FUN_1008_a626
   ============================================================ */

void __cdecl16far FUN_1008_a626(void)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  
  for (iVar3 = 0; iVar3 < 0x28; iVar3 = iVar3 + 1) {
    for (iVar2 = 0; iVar2 < 0x24; iVar2 = iVar2 + 1) {
      pbVar1 = (byte *)((iVar3 * 0x24 + iVar2) * 2);
      *pbVar1 = *pbVar1 & 0xfe;
    }
  }
  return;
}



/* ============================================================
   1008:a652  FUN_1008_a652
   ============================================================ */

void __stdcall16far FUN_1008_a652(int param_1,undefined2 param_2)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  
  if (DAT_1080_1070 != 0) {
    for (iVar3 = 0; iVar3 < 0x28; iVar3 = iVar3 + 1) {
      for (iVar2 = 0; iVar2 < 0x24; iVar2 = iVar2 + 1) {
        pbVar1 = (byte *)((iVar3 * 0x24 + iVar2) * 2);
        *pbVar1 = *pbVar1 & 0xf9;
      }
    }
    for (iVar3 = 0; iVar3 < *(int *)(param_1 + 0x50); iVar3 = iVar3 + 1) {
      uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x2c) >> 0x10);
      iVar2 = (int)*(undefined4 *)(param_1 + 0x2c) + iVar3 * 0xc;
      pbVar1 = (byte *)((*(int *)(iVar2 + 2) * 0x24 + *(int *)(iVar2 + 4)) * 2);
      *pbVar1 = *pbVar1 | 2;
    }
    for (iVar3 = 0; iVar3 < *(int *)(param_1 + 0x52); iVar3 = iVar3 + 1) {
      uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x32) >> 0x10);
      iVar2 = (int)*(undefined4 *)(param_1 + 0x32) + iVar3 * 10;
      pbVar1 = (byte *)((*(int *)(iVar2 + 2) * 0x24 + *(int *)(iVar2 + 4)) * 2);
      *pbVar1 = *pbVar1 | 4;
    }
    DAT_1080_1070 = 0;
  }
  return;
}



/* ============================================================
   1008:a704  FUN_1008_a704
   ============================================================ */

bool __stdcall16far FUN_1008_a704(undefined2 param_1,undefined2 param_2,int param_3,int param_4)

{
  FUN_1008_a652(param_1,param_2);
  return (*(byte *)((param_4 * 0x24 + param_3) * 2) & 2) != 0;
}



/* ============================================================
   1008:a732  FUN_1008_a732
   ============================================================ */

void __stdcall16far FUN_1008_a732(int param_1,undefined2 param_2)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  
  bVar1 = false;
  for (iVar3 = 0; iVar3 < *(int *)(param_1 + 0x52); iVar3 = iVar3 + 1) {
    if (*(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar3 * 10) == 3) {
      bVar1 = true;
      uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x32) >> 0x10);
      iVar2 = (int)*(undefined4 *)(param_1 + 0x32) + iVar3 * 10;
      FUN_1008_a7a8(param_1,param_2,*(undefined2 *)(iVar2 + 4),*(undefined2 *)(iVar2 + 2));
      FUN_1008_7f8e(param_1,param_2,iVar3);
      iVar3 = iVar3 + -1;
    }
  }
  if (bVar1) {
    uVar4 = 0x18;
  }
  else {
    uVar4 = 0x17;
  }
  FUN_1008_31d4(*(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6),uVar4);
  return;
}



/* ============================================================
   1008:a7a8  FUN_1008_a7a8
   ============================================================ */

void __stdcall16far FUN_1008_a7a8(undefined2 param_1,undefined2 param_2,int param_3,int param_4)

{
  int iVar1;
  
  FUN_1008_a980(param_1,param_2,param_3,param_4);
  iVar1 = FUN_1008_a980(param_1,param_2,param_3 + -1,param_4 + -1);
  if (iVar1 != 0) {
    FUN_1008_a980(param_1,param_2,param_3 + -1,param_4 + -2);
    FUN_1008_a980(param_1,param_2,param_3 + -2,param_4 + -1);
  }
  iVar1 = FUN_1008_a980(param_1,param_2,param_3 + -1,param_4 + 1);
  if (iVar1 != 0) {
    FUN_1008_a980(param_1,param_2,param_3 + -1,param_4 + 2);
    FUN_1008_a980(param_1,param_2,param_3 + -2,param_4 + 1);
  }
  iVar1 = FUN_1008_a980(param_1,param_2,param_3 + 1,param_4 + -1);
  if (iVar1 != 0) {
    FUN_1008_a980(param_1,param_2,param_3 + 1,param_4 + -2);
    FUN_1008_a980(param_1,param_2,param_3 + 2,param_4 + -1);
  }
  iVar1 = FUN_1008_a980(param_1,param_2,param_3 + 1,param_4 + 1);
  if (iVar1 != 0) {
    FUN_1008_a980(param_1,param_2,param_3 + 1,param_4 + 2);
    FUN_1008_a980(param_1,param_2,param_3 + 2,param_4 + 1);
  }
  iVar1 = FUN_1008_a980(param_1,param_2,param_3 + -1,param_4);
  if (iVar1 != 0) {
    FUN_1008_a980(param_1,param_2,param_3 + -2,param_4);
  }
  iVar1 = FUN_1008_a980(param_1,param_2,param_3,param_4 + -1);
  if (iVar1 != 0) {
    FUN_1008_a980(param_1,param_2,param_3,param_4 + -2);
  }
  iVar1 = FUN_1008_a980(param_1,param_2,param_3,param_4 + 1);
  if (iVar1 != 0) {
    FUN_1008_a980(param_1,param_2,param_3,param_4 + 2);
  }
  iVar1 = FUN_1008_a980(param_1,param_2,param_3 + 1,param_4);
  if (iVar1 != 0) {
    FUN_1008_a980(param_1,param_2,param_3 + 2,param_4);
  }
  return;
}



/* ============================================================
   1008:a980  FUN_1008_a980
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_a980(undefined4 *param_1,undefined2 param_2,int param_3,int param_4)

{
  undefined2 *puVar1;
  undefined4 uVar2;
  int iVar3;
  undefined2 uVar4;
  uint uVar5;
  int iVar6;
  
  iVar3 = FUN_1008_91c6(param_1,param_2,param_3,param_4);
  if (iVar3 == 0) {
LAB_1008_a99d:
    uVar4 = 0;
  }
  else {
    uVar2 = *(undefined4 *)((int)param_1[2] + param_4 * 4);
    uVar5 = FUN_1008_916e(param_1,param_2,*(undefined2 *)((int)uVar2 + param_3 * 2));
    if ((uVar5 & 2) != 0) {
      uVar2 = *(undefined4 *)((int)param_1[2] + param_4 * 4);
      uVar5 = FUN_1008_916e(param_1,param_2,*(undefined2 *)((int)uVar2 + param_3 * 2));
      if ((uVar5 & 4) != 0) goto LAB_1008_a99d;
    }
    FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),0x14,param_3,
                  param_4);
    iVar3 = FUN_1008_917e(param_1,param_2,param_3,param_4);
    if (iVar3 == 0) {
      puVar1 = (undefined2 *)((int)*param_1 + 0x40);
      (*(code *)*puVar1)((char *)s_Lucky_Chip_1080_1004 + 4,param_1,param_2,param_3,param_4,3);
    }
    for (iVar3 = 0; iVar3 < *(int *)(param_1 + 0x14); iVar3 = iVar3 + 1) {
      if ((*(int *)((int)param_1[0xb] + iVar3 * 0xc + 2) == param_4) &&
         (*(int *)((int)param_1[0xb] + iVar3 * 0xc + 4) == param_3)) {
        if (*(int *)((int)param_1[0xb] + iVar3 * 0xc) == 2) {
          iVar6 = FUN_1008_3c88(10);
          if (iVar6 == 2) {
            FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),1,
                          param_3,param_4);
          }
        }
        FUN_1008_ad16(param_1,param_2,*(undefined2 *)((int)param_1[0xb] + iVar3 * 0xc));
        FUN_1008_8032(param_1,param_2,iVar3);
        iVar3 = iVar3 + -1;
      }
    }
    uVar4 = 1;
  }
  return uVar4;
}



/* ============================================================
   1008:aacc  FUN_1008_aacc
   ============================================================ */

void __stdcall16far FUN_1008_aacc(undefined4 *param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  undefined2 *puVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  undefined4 *puVar9;
  int iVar10;
  int *piVar11;
  int iVar12;
  undefined2 uVar13;
  undefined2 uVar14;
  int local_10;
  int local_8;
  int local_6;
  
  uVar13 = (undefined2)((ulong)param_1 >> 0x10);
  puVar9 = (undefined4 *)param_1;
  uVar14 = (undefined2)((ulong)*(undefined4 *)((int)puVar9 + 0x36) >> 0x10);
  iVar10 = (int)*(undefined4 *)((int)puVar9 + 0x36);
  local_10 = *(int *)(iVar10 + 4);
  piVar11 = (int *)*(undefined2 *)(iVar10 + 2);
  while (local_10 != 0 || piVar11 != (int *)0x0) {
    if (piVar11[2] == 0x24) {
      iVar10 = *piVar11;
      iVar2 = piVar11[1];
      for (local_8 = iVar10 + -1; local_8 <= iVar10 + 1; local_8 = local_8 + 1) {
        for (iVar12 = iVar2 + -1; iVar12 <= iVar2 + 1; iVar12 = iVar12 + 1) {
          iVar6 = FUN_1008_91c6(puVar9,uVar13,iVar12,local_8);
          if (iVar6 != 0) {
            iVar6 = FUN_1008_917e(puVar9,uVar13,iVar12,local_8);
            if ((iVar6 != 0) ||
               ((uVar5 = *(undefined4 *)((int)puVar9[2] + local_8 * 4),
                uVar7 = FUN_1008_916e(puVar9,uVar13,*(undefined2 *)((int)uVar5 + iVar12 * 2)),
                (uVar7 & 2) != 0 &&
                (uVar5 = *(undefined4 *)((int)puVar9[2] + local_8 * 4),
                uVar7 = FUN_1008_916e(puVar9,uVar13,*(undefined2 *)((int)uVar5 + iVar12 * 2)),
                (uVar7 & 4) == 0)))) {
              FUN_1008_af5a(*(undefined2 *)((int)puVar9 + 0x36),*(undefined2 *)(puVar9 + 0xe),0x14,
                            iVar12,local_8);
              iVar6 = FUN_1008_917e(puVar9,uVar13,iVar12,local_8);
              if (iVar6 == 0) {
                puVar4 = (undefined2 *)((int)*param_1 + 0x40);
                (*(code *)*puVar4)((char *)s_Lucky_Chip_1080_1004 + 4,puVar9,uVar13,iVar12,local_8,1
                                  );
              }
            }
            FUN_1008_a060(puVar9,uVar13,iVar12,local_8);
            local_6 = *(int *)(puVar9 + 0x14);
            for (iVar6 = 0; iVar6 < local_6; iVar6 = iVar6 + 1) {
              if ((*(int *)((int)puVar9[0xb] + iVar6 * 0xc + 2) == local_8) &&
                 (*(int *)((int)puVar9[0xb] + iVar6 * 0xc + 4) == iVar12)) {
                if ((*(int *)((int)puVar9[0xb] + iVar6 * 0xc) == 2) &&
                   (iVar8 = FUN_1008_3c88(10), iVar8 == 2)) {
                  FUN_1008_af5a(*(undefined2 *)((int)puVar9 + 0x36),*(undefined2 *)(puVar9 + 0xe),1,
                                iVar12,local_8);
                }
                if (*(int *)((int)puVar9[0xb] + iVar6 * 0xc) == 3) {
                  FUN_1008_bc2c(puVar9,uVar13,iVar12,local_8);
                }
                FUN_1008_ad16(puVar9,uVar13,*(undefined2 *)((int)puVar9[0xb] + iVar6 * 0xc));
                FUN_1008_8032(puVar9,uVar13,iVar6);
                local_6 = local_6 + -1;
                iVar6 = iVar6 + -1;
              }
            }
          }
        }
      }
      FUN_1008_31d4(*(undefined2 *)(puVar9 + 1),*(undefined2 *)((int)puVar9 + 6),0xc);
      piVar3 = (int *)piVar11[3];
      local_10 = piVar11[4];
      FUN_1008_afb0(*(undefined2 *)((int)puVar9 + 0x36),*(undefined2 *)(puVar9 + 0xe),0x24,iVar2,
                    iVar10);
      piVar11 = piVar3;
    }
    else {
      piVar1 = piVar11 + 3;
      local_10 = piVar11[4];
      piVar11 = (int *)*piVar1;
    }
  }
  return;
}



/* ============================================================
   1008:ad16  FUN_1008_ad16
   ============================================================ */

void __stdcall16far FUN_1008_ad16(undefined4 param_1,int param_2)

{
  undefined2 uVar1;
  undefined2 uVar2;
  
  if (param_2 == 0) {
    uVar2 = 10;
  }
  else if (param_2 == 1) {
    uVar2 = 0xf;
  }
  else if (param_2 == 2) {
    uVar2 = 100;
  }
  else if (param_2 == 3) {
    uVar2 = 0xfa;
  }
  else {
    if (param_2 != 4) {
      return;
    }
    uVar2 = 5;
  }
  uVar1 = (undefined2)((ulong)param_1 >> 0x10);
  FUN_1008_2840(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),uVar2,0);
  return;
}



/* ============================================================
   1008:adee  FUN_1008_adee
   ============================================================ */

int * __stdcall16far FUN_1008_adee(int *param_1,undefined4 param_2)

{
  undefined4 uVar1;
  int *piVar2;
  int in_DX;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  
  uVar7 = (undefined2)((ulong)param_1 >> 0x10);
  piVar3 = (int *)param_1;
  piVar3[1] = 0;
  *param_1 = 0;
  iVar6 = 0;
  do {
    for (iVar5 = 0; iVar5 < 0x24; iVar5 = iVar5 + 1) {
      uVar8 = (undefined2)((ulong)param_2 >> 0x10);
      iVar4 = (int)param_2;
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + iVar6 * 4);
      if (((*(int *)((int)uVar1 + iVar5 * 2) == 7) ||
          (uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + iVar6 * 4),
          *(int *)((int)uVar1 + iVar5 * 2) == 0x74)) ||
         (uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + iVar6 * 4),
         *(int *)((int)uVar1 + iVar5 * 2) == 0x7c)) {
        piVar2 = (int *)FUN_1000_1646(8);
        *piVar2 = iVar6;
        piVar2[1] = iVar5;
        iVar4 = piVar3[1];
        piVar2[2] = *param_1;
        piVar2[3] = iVar4;
        *param_1 = (int)piVar2;
        piVar3[1] = in_DX;
      }
    }
    iVar6 = iVar6 + 1;
  } while (iVar6 < 0x28);
  return param_1;
}



/* ============================================================
   1008:aeba  FUN_1008_aeba
   ============================================================ */

void __stdcall16far FUN_1008_aeba(int *param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  undefined2 uVar3;
  
  while (param_1[1] != 0 || *param_1 != 0) {
    uVar3 = (undefined2)((ulong)*(undefined4 *)param_1 >> 0x10);
    iVar2 = (int)*(undefined4 *)param_1;
    iVar1 = *(int *)(iVar2 + 6);
    *param_1 = *(int *)(iVar2 + 4);
    param_1[1] = iVar1;
    FUN_1000_1634(iVar2,uVar3);
  }
  return;
}



/* ============================================================
   1008:aefa  FUN_1008_aefa
   ============================================================ */

undefined2 * __stdcall16far FUN_1008_aefa(undefined2 *param_1,undefined2 param_2)

{
  param_1[2] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1008:af18  FUN_1008_af18
   ============================================================ */

void __stdcall16far FUN_1008_af18(int param_1,undefined2 param_2)

{
  undefined2 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  while (*(int *)(param_1 + 4) != 0 || *(int *)(param_1 + 2) != 0) {
    uVar3 = (undefined2)((ulong)*(undefined4 *)(param_1 + 2) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 2);
    uVar1 = *(undefined2 *)(iVar2 + 8);
    *(undefined2 *)(param_1 + 2) = *(undefined2 *)(iVar2 + 6);
    *(undefined2 *)(param_1 + 4) = uVar1;
    FUN_1000_1634(iVar2,uVar3);
  }
  return;
}



/* ============================================================
   1008:af5a  FUN_1008_af5a
   ============================================================ */

void __stdcall16far
FUN_1008_af5a(int *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  int iVar1;
  undefined2 *puVar2;
  int in_DX;
  
  puVar2 = (undefined2 *)FUN_1000_1646(10);
  *puVar2 = param_5;
  puVar2[1] = param_4;
  puVar2[2] = param_3;
  iVar1 = param_1[2];
  puVar2[3] = param_1[1];
  puVar2[4] = iVar1;
  param_1[1] = (int)puVar2;
  param_1[2] = in_DX;
  *param_1 = *param_1 + 1;
  return;
}



/* ============================================================
   1008:afb0  FUN_1008_afb0
   ============================================================ */

undefined2 __stdcall16far FUN_1008_afb0(int *param_1,int param_2,int param_3,int param_4)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  
  uVar6 = (undefined2)((ulong)param_1 >> 0x10);
  piVar3 = (int *)param_1;
  if (((**(int **)(piVar3 + 1) == param_4) &&
      (*(int *)((int)*(undefined4 *)(piVar3 + 1) + 2) == param_3)) &&
     (*(int *)((int)*(undefined4 *)(piVar3 + 1) + 4) == param_2)) {
    uVar7 = (undefined2)((ulong)*(undefined4 *)(piVar3 + 1) >> 0x10);
    iVar4 = (int)*(undefined4 *)(piVar3 + 1);
    iVar2 = *(int *)(iVar4 + 8);
    piVar3[1] = *(int *)(iVar4 + 6);
    piVar3[2] = iVar2;
LAB_1008_b008:
    FUN_1000_1634(iVar4,uVar7);
    uVar6 = 1;
    *param_1 = *param_1 + -1;
  }
  else {
    iVar2 = piVar3[2];
    iVar5 = piVar3[1];
    while (*(int *)(iVar5 + 8) != 0 || *(int *)(iVar5 + 6) != 0) {
      if (((*(int *)*(undefined4 *)(iVar5 + 6) == param_4) &&
          (*(int *)((int)*(undefined4 *)(iVar5 + 6) + 2) == param_3)) &&
         (*(int *)((int)*(undefined4 *)(iVar5 + 6) + 4) == param_2)) {
        uVar7 = (undefined2)((ulong)*(undefined4 *)(iVar5 + 6) >> 0x10);
        iVar4 = (int)*(undefined4 *)(iVar5 + 6);
        uVar6 = *(undefined2 *)(iVar4 + 8);
        *(undefined2 *)(iVar5 + 6) = *(undefined2 *)(iVar4 + 6);
        *(undefined2 *)(iVar5 + 8) = uVar6;
        goto LAB_1008_b008;
      }
      piVar1 = (int *)(iVar5 + 6);
      iVar2 = *(int *)(iVar5 + 8);
      iVar5 = *piVar1;
    }
    uVar6 = 0;
  }
  return uVar6;
}



/* ============================================================
   1008:b09e  FUN_1008_b09e
   ============================================================ */

void __stdcall16far
FUN_1008_b09e(undefined4 param_1,undefined2 param_2,undefined2 param_3,undefined4 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  undefined2 unaff_SS;
  undefined2 uVar9;
  
  uVar9 = (undefined2)((ulong)param_4 >> 0x10);
  iVar2 = *(int *)((int)param_4 + 0xea);
  iVar3 = *(int *)((int)param_4 + 0xec);
  uVar9 = (undefined2)((ulong)param_1 >> 0x10);
  iVar7 = *(int *)((int)param_1 + 4);
  piVar8 = (int *)*(undefined2 *)((int)param_1 + 2);
  while (iVar7 != 0 || piVar8 != (int *)0x0) {
    iVar4 = *piVar8;
    iVar5 = piVar8[1];
    if ((((*(byte *)((iVar5 + iVar4 * 0x24) * 2) & 1) != 0) &&
        ((*(byte *)((iVar4 * 0x24 + iVar5) * 2) & 6) == 0)) &&
       ((iVar4 != iVar2 || (iVar5 != iVar3)))) {
      iVar6 = FUN_1008_917e(param_2,param_3,iVar5,iVar4);
      if ((iVar6 != 0) && (*(int *)(piVar8[2] * 2 + 0x1524) != -1)) {
        FUN_1008_3bd0(param_5,param_6,iVar5,iVar4,*(undefined2 *)(piVar8[2] * 2 + 0x1524));
      }
    }
    piVar1 = piVar8 + 3;
    iVar7 = piVar8[4];
    piVar8 = (int *)*piVar1;
  }
  return;
}



/* ============================================================
   1008:b164  FUN_1008_b164
   ============================================================ */

char * __stdcall16far FUN_1008_b164(undefined4 param_1,int param_2,undefined2 param_3)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  undefined4 uVar3;
  int iVar4;
  char *pcVar5;
  undefined2 *puVar6;
  int iVar7;
  undefined2 *puVar8;
  undefined2 unaff_SS;
  undefined2 uVar9;
  undefined2 local_1e [12];
  int local_6;
  int local_4;
  
  pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
  puVar8 = local_1e;
  puVar6 = (undefined2 *)0xd2c8;
  for (iVar4 = 0xb; iVar4 != 0; iVar4 = iVar4 + -1) {
    puVar2 = puVar8;
    puVar8 = puVar8 + 1;
    puVar1 = puVar6;
    puVar6 = puVar6 + 1;
    *puVar2 = *puVar1;
  }
  *(undefined1 *)puVar8 = *(undefined1 *)puVar6;
  for (iVar4 = 0; iVar4 < 0x28; iVar4 = iVar4 + 1) {
    for (iVar7 = 0; iVar7 < 0x24; iVar7 = iVar7 + 1) {
      pcVar5 = (char *)(iVar7 * 2);
      uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
      (pcVar5 + (int)uVar3)[0] = '\0';
      (pcVar5 + (int)uVar3)[1] = '\0';
    }
  }
  iVar4 = 0;
  do {
    local_4 = iVar4;
    for (iVar7 = 0; iVar7 < 0x24; iVar7 = iVar7 + 1) {
      pcVar5 = (char *)(iVar7 * 0x29 + param_2);
      switch(pcVar5[iVar4]) {
      case ' ':
        break;
      default:
        local_6 = FUN_1000_1710(local_1e);
        (&stack0xffe1)[local_6] = *(undefined1 *)(iVar7 * 0x29 + param_2 + iVar4);
        FUN_1008_2d02(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0,local_1e
                      ,unaff_SS);
        goto switchD_1008_b1e2_caseD_2e;
      case '$':
        uVar9 = 1;
        goto LAB_1008_b305;
      case '&':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = 'C';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '(':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '8';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case ')':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '9';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '*':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '\x03';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '+':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '@';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '-':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '6';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '.':
        goto switchD_1008_b1e2_caseD_2e;
      case '/':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = 'A';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '0':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '\x01';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case ';':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = 'm';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '<':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '#';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '>':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '\'';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '?':
        uVar9 = 2;
LAB_1008_b305:
        FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),
                      uVar9,iVar7,iVar4);
        goto switchD_1008_b1e2_caseD_2e;
      case '@':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '\a';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'B':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = ']';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'C':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '\\';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'D':
        uVar9 = 0x1a;
        goto LAB_1008_b4d4;
      case 'E':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '<';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'G':
        uVar9 = 0x18;
        goto LAB_1008_b4d4;
      case 'I':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = -0x4f;
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'K':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '~';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'O':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = -0x50;
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'R':
        uVar9 = 0x1c;
LAB_1008_b4d4:
        pcVar5 = (char *)FUN_1008_c1e6((int)param_1,param_1._2_2_,0,uVar9,iVar7,iVar4);
        break;
      case 'S':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = 'l';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'T':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '?';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'U':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = -0x51;
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'W':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = 'n';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'Z':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = ' ';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'b':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '`';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'c':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '_';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'd':
        uVar9 = 2;
        goto LAB_1008_b648;
      case 'g':
        uVar9 = 0;
        goto LAB_1008_b648;
      case 'm':
        uVar9 = 1;
        goto LAB_1008_b648;
      case 'n':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = 'o';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'o':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '\x02';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 's':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = 'h';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 't':
        pcVar5 = (char *)FUN_1008_c452((int)param_1,param_1._2_2_,iVar7,iVar4);
        break;
      case 'w':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '>';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case 'z':
        uVar9 = 4;
LAB_1008_b648:
        FUN_1008_80c0((int)param_1,param_1._2_2_,0,0,0,1,iVar7,iVar4,uVar9);
switchD_1008_b1e2_caseD_2e:
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '\0';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '{':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = ':';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '|':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '7';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '}':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = ';';
        (pcVar5 + (int)uVar3)[1] = '\0';
        break;
      case '~':
        pcVar5 = (char *)(iVar7 * 2);
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        (pcVar5 + (int)uVar3)[0] = '=';
        (pcVar5 + (int)uVar3)[1] = '\0';
      }
    }
    iVar4 = iVar4 + 1;
    if (0x27 < iVar4) {
      return pcVar5;
    }
  } while( true );
}



/* ============================================================
   1008:b78a  FUN_1008_b78a
   ============================================================ */

void __stdcall16far FUN_1008_b78a(int param_1,undefined2 param_2,int param_3)

{
  undefined2 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  char *pcVar6;
  undefined2 unaff_SS;
  undefined1 local_26 [4];
  int local_22;
  undefined1 local_14 [6];
  undefined2 local_e;
  undefined2 local_c;
  undefined2 local_a;
  undefined2 local_8;
  int local_6;
  undefined2 local_4;
  
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0xc);
  uVar1 = *(undefined2 *)((int)uVar2 + 8);
  local_c = GETDC();
  FUN_1008_3b6c(local_14,unaff_SS,local_c);
  uVar5 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar4 = (int)*(undefined4 *)(param_1 + 4);
  local_8 = *(undefined2 *)(iVar4 + 0xea);
  local_a = *(undefined2 *)(iVar4 + 0xec);
  iVar4 = SETTIMER((char *)s_Lucky_Chip_1080_1004 + 4,0,0,0x32,0xd);
  uVar5 = local_8;
  if (iVar4 == 0) {
    pcVar6 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_2d02(*(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6),0,0xd13a,
                  (char *)s_Lucky_Chip_1080_1004 + 4);
  }
  else {
    local_4 = 1;
    iVar4 = 1;
    pcVar6 = (char *)0x11d0;
    local_e = uVar1;
    while (iVar4 < 6) {
      iVar3 = PEEKMESSAGE(pcVar6,1,0x113,0x113,0,local_26,unaff_SS);
      pcVar6 = (char *)0x11d0;
      if ((iVar3 != 0) && (local_22 == 0xd)) {
        if (param_3 == 0) {
          local_6 = iVar4 + 0x2f;
        }
        else {
          local_6 = iVar4 + 0x34;
        }
        pcVar6 = (char *)s_Lucky_Chip_1080_1004 + 4;
        FUN_1008_3bd0(local_14,unaff_SS,local_a,uVar5,local_6);
        iVar4 = iVar4 + 1;
      }
    }
    FUN_1008_a060(param_1,param_2,local_a,local_8);
    pcVar6 = (char *)0x11d0;
    KILLTIMER((char *)s_Lucky_Chip_1080_1004 + 4,0xd);
  }
  RELEASEDC(pcVar6,local_c);
  FUN_1008_3ba6(local_14,unaff_SS);
  return;
}



/* ============================================================
   1008:b946  FUN_1008_b946
   ============================================================ */

int * __stdcall16far FUN_1008_b946(undefined4 param_1,int param_2,int *param_3,undefined2 param_4)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  for (iVar3 = 0; iVar3 < 0x28; iVar3 = iVar3 + 1) {
    for (iVar2 = 0; iVar2 < 0x24; iVar2 = iVar2 + 1) {
      uVar1 = *(undefined4 *)((int)param_1 + 8);
      uVar1 = *(undefined4 *)((int)uVar1 + iVar3 * 4);
      if (*(int *)((int)uVar1 + iVar2 * 2) == param_2 + 0x23) goto LAB_1008_b9a8;
    }
  }
  iVar3 = 0;
  iVar2 = 0;
LAB_1008_b9a8:
  *param_3 = iVar3;
  param_3[1] = iVar2;
  return (int *)CONCAT22(param_4,param_3);
}



/* ============================================================
   1008:b9ba  FUN_1008_b9ba
   ============================================================ */

int * __stdcall16far FUN_1008_b9ba(undefined4 param_1,int param_2,int *param_3,undefined2 param_4)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  for (iVar3 = 0; iVar3 < 0x28; iVar3 = iVar3 + 1) {
    for (iVar2 = 0; iVar2 < 0x24; iVar2 = iVar2 + 1) {
      uVar1 = *(undefined4 *)((int)param_1 + 8);
      uVar1 = *(undefined4 *)((int)uVar1 + iVar3 * 4);
      if (*(int *)((int)uVar1 + iVar2 * 2) == param_2 + 0x27) goto LAB_1008_ba1f;
    }
  }
  iVar3 = -1;
  iVar2 = -1;
LAB_1008_ba1f:
  *param_3 = iVar3;
  param_3[1] = iVar2;
  return (int *)CONCAT22(param_4,param_3);
}



/* ============================================================
   1008:ba30  FUN_1008_ba30
   ============================================================ */

void __stdcall16far FUN_1008_ba30(undefined4 *param_1,int param_2,int param_3,int param_4)

{
  uint *puVar1;
  uint uVar2;
  undefined2 *puVar3;
  undefined4 uVar4;
  char cVar5;
  undefined4 *puVar6;
  int iVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  
  uVar8 = (undefined2)((ulong)param_1 >> 0x10);
  puVar6 = (undefined4 *)param_1;
  puVar1 = (uint *)((int)puVar6 + 0x3e);
  uVar2 = *puVar1;
  *puVar1 = *puVar1 + 1;
  *(uint *)(puVar6 + 0x10) = *(int *)(puVar6 + 0x10) + (uint)(0xfffe < uVar2);
  FUN_1008_aacc(puVar6,uVar8);
  uVar9 = (undefined2)((ulong)puVar6[1] >> 0x10);
  iVar7 = (int)puVar6[1];
  uVar4 = *(undefined4 *)(*(int *)(iVar7 + 0xea) * 4 + (int)puVar6[2]);
  uVar2 = *(uint *)((int)uVar4 + *(int *)(iVar7 + 0xec) * 2);
  if (uVar2 == 0x7e) {
    if (((param_4 != 0) || (param_3 != 0)) && (param_2 == 0)) {
      FUN_1008_2822(*(undefined2 *)(puVar6 + 1),*(undefined2 *)((int)puVar6 + 6),0xd30c,
                    (char *)s_Lucky_Chip_1080_1004 + 4);
      puVar3 = (undefined2 *)((int)*param_1 + 0x34);
      (*(code *)*puVar3)((char *)s_Lucky_Chip_1080_1004 + 4,puVar6,uVar8);
    }
  }
  else if (uVar2 < 0x7f) {
    cVar5 = (char)uVar2;
    if ('\"' < cVar5) {
      if (cVar5 == '&' || (char)(cVar5 + -0x23) < '\x03') {
        if (((param_4 != 0) || (param_3 != 0)) && (param_2 == 0)) {
          FUN_1008_b78a(puVar6,uVar8,1);
          puVar3 = (undefined2 *)((int)*param_1 + 0x18);
          (*(code *)*puVar3)((char *)s_Lucky_Chip_1080_1004 + 4,puVar6,uVar8,uVar2);
        }
      }
      else if ((('\0' < (char)(cVar5 + -0x26)) && ((char)(cVar5 + -0x27) < '\x04')) &&
              (((param_4 != 0 || (param_3 != 0)) && (param_2 == 0)))) {
        FUN_1008_b78a(puVar6,uVar8,0);
        puVar3 = (undefined2 *)((int)*param_1 + 0x14);
        (*(code *)*puVar3)((char *)s_Lucky_Chip_1080_1004 + 4,puVar6,uVar8,uVar2);
      }
    }
  }
  FUN_1008_31d4(*(undefined2 *)(puVar6 + 1),*(undefined2 *)((int)puVar6 + 6),0x20);
  return;
}



/* ============================================================
   1008:bb5a  FUN_1008_bb5a
   ============================================================ */

void __stdcall16far
FUN_1008_bb5a(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  FUN_1008_bb7e(param_1,param_2,param_3,param_4,param_5,0x23,0x27,0,0);
  return;
}



/* ============================================================
   1008:bb7e  FUN_1008_bb7e
   ============================================================ */

void __stdcall16far
FUN_1008_bb7e(undefined4 param_1,int param_2,undefined2 param_3,int param_4,int param_5,int param_6,
             int param_7,int param_8)

{
  undefined4 uVar1;
  int iVar2;
  
  while( true ) {
    iVar2 = param_7;
    if (param_6 < param_8) break;
    for (; iVar2 <= param_5; iVar2 = iVar2 + 1) {
      uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_8 * 4);
      if (*(int *)((int)uVar1 + iVar2 * 2) == param_4) {
        if (param_2 != 0) {
          FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),
                        0x14,iVar2,param_8);
        }
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_8 * 4);
        *(undefined2 *)((int)uVar1 + iVar2 * 2) = param_3;
        FUN_1008_a060((int)param_1,param_1._2_2_,iVar2,param_8);
      }
    }
    param_8 = param_8 + 1;
  }
  if (param_2 != 0) {
    uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xc);
    FUN_1008_8b2c((int)param_1,param_1._2_2_,*(undefined2 *)((int)uVar1 + 8));
  }
  return;
}



/* ============================================================
   1008:bc2c  FUN_1008_bc2c
   ============================================================ */

void __stdcall16far
FUN_1008_bc2c(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  undefined2 uVar1;
  undefined2 uVar2;
  
  uVar2 = 2;
  uVar1 = FUN_1008_3c88(7,param_3,param_4,2);
  FUN_1008_80c0(param_1,param_2,0,0,0,uVar1,param_3,param_4,uVar2);
  return;
}



/* ============================================================
   1008:bed2  FUN_1008_bed2
   ============================================================ */

undefined2 __stdcall16far FUN_1008_bed2(void)

{
  int iVar1;
  int in_stack_0000000c;
  
  if (in_stack_0000000c == 0x13) {
    iVar1 = FUN_1008_3c88(9);
    if ((0 < iVar1) && (!SBORROW2(iVar1,1))) {
      if (iVar1 == 4 || iVar1 + -1 < 3) {
        return 1;
      }
      if ((0 < iVar1 + -4) && (iVar1 + -5 < 4)) {
        return 2;
      }
    }
  }
  else if (in_stack_0000000c == 4) {
    iVar1 = FUN_1008_3c88(0x14);
    if (iVar1 == 8) {
      return 1;
    }
    if (iVar1 == 9) {
      return 2;
    }
    if (iVar1 == 10) {
      return 7;
    }
  }
  return 0xffff;
}



/* ============================================================
   1008:bf2a  FUN_1008_bf2a
   ============================================================ */

void __stdcall16far FUN_1008_bf2a(undefined4 param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined2 uVar3;
  
  uVar3 = (undefined2)((ulong)param_1 >> 0x10);
  param_2 = param_2 * 2;
  puVar1 = (undefined4 *)((int)param_1 + 8);
  uVar2 = *(undefined4 *)((int)param_1 + 8);
  param_3 = param_3 * 4;
  uVar2 = *(undefined4 *)((int)uVar2 + param_3);
  *(undefined2 *)((int)uVar2 + param_2) = 0x45;
  uVar2 = *(undefined4 *)((int)*puVar1 + param_3 + 4);
  *(undefined2 *)((int)uVar2 + param_2) = 0x46;
  uVar2 = *(undefined4 *)((int)*puVar1 + param_3 + 8);
  *(undefined2 *)((int)uVar2 + param_2) = 0x47;
  uVar2 = *(undefined4 *)((int)*puVar1 + param_3);
  *(undefined2 *)((int)uVar2 + param_2 + 2) = 0x48;
  uVar2 = *(undefined4 *)((int)*puVar1 + param_3 + 4);
  *(undefined2 *)((int)uVar2 + param_2 + 2) = 0x49;
  uVar2 = *(undefined4 *)((int)*puVar1 + param_3 + 8);
  *(undefined2 *)((int)uVar2 + param_2 + 2) = 0x4d;
  uVar2 = *(undefined4 *)((int)*puVar1 + param_3);
  *(undefined2 *)((int)uVar2 + param_2 + 4) = 0x4e;
  uVar2 = *(undefined4 *)((int)*puVar1 + param_3 + 4);
  *(undefined2 *)((int)uVar2 + param_2 + 4) = 0x4f;
  uVar2 = *(undefined4 *)((int)*puVar1 + param_3 + 8);
  *(undefined2 *)((int)uVar2 + param_2 + 4) = 0x50;
  return;
}



/* ============================================================
   1008:c1e6  FUN_1008_c1e6
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_c1e6(undefined4 param_1,int param_2,undefined2 param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = param_5;
  do {
    iVar3 = param_4;
    if (param_5 + 3 <= iVar4) {
      switch(param_3) {
      case 0x18:
      case 0x1a:
      case 0x1c:
      case 0x1e:
        iVar4 = param_4 * 2;
        iVar3 = param_5 * 4;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3);
        *(undefined2 *)((int)uVar1 + iVar4) = 8;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 4);
        *(undefined2 *)((int)uVar1 + iVar4) = 9;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 8);
        *(undefined2 *)((int)uVar1 + iVar4) = 10;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3);
        *(undefined2 *)((int)uVar1 + iVar4 + 2) = 0xb;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 8);
        *(undefined2 *)((int)uVar1 + iVar4 + 2) = 0xc;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3);
        *(undefined2 *)((int)uVar1 + iVar4 + 4) = 0xd;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 4);
        *(undefined2 *)((int)uVar1 + iVar4 + 4) = 0xe;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 8);
        *(undefined2 *)((int)uVar1 + iVar4 + 4) = 0xf;
        break;
      case 0x19:
      case 0x1b:
      case 0x1d:
      case 0x1f:
        iVar4 = param_4 * 2;
        iVar3 = param_5 * 4;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3);
        *(undefined2 *)((int)uVar1 + iVar4) = 0x10;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 4);
        *(undefined2 *)((int)uVar1 + iVar4) = 0x11;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 8);
        *(undefined2 *)((int)uVar1 + iVar4) = 0x12;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3);
        *(undefined2 *)((int)uVar1 + iVar4 + 2) = 0x13;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 8);
        *(undefined2 *)((int)uVar1 + iVar4 + 2) = 0x14;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3);
        *(undefined2 *)((int)uVar1 + iVar4 + 4) = 0x15;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 4);
        *(undefined2 *)((int)uVar1 + iVar4 + 4) = 0x16;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 + 8);
        *(undefined2 *)((int)uVar1 + iVar4 + 4) = 0x17;
      }
      uVar1 = *(undefined4 *)(param_5 * 4 + (int)*(undefined4 *)((int)param_1 + 8) + 4);
      *(undefined2 *)((int)uVar1 + param_4 * 2 + 2) = param_3;
      return 1;
    }
    for (; iVar3 < param_4 + 3; iVar3 = iVar3 + 1) {
      iVar2 = FUN_1008_91c6((int)param_1,param_1._2_2_,iVar3,iVar4);
      if ((iVar2 == 0) ||
         ((uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4),
          *(int *)((int)uVar1 + iVar3 * 2) != 0 && (param_2 == 0)))) {
        return 0;
      }
    }
    iVar4 = iVar4 + 1;
  } while( true );
}



/* ============================================================
   1008:c40a  FUN_1008_c40a
   ============================================================ */

void __stdcall16far
FUN_1008_c40a(int param_1,undefined2 param_2,undefined2 param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_5 * 4);
  if (*(int *)((int)uVar1 + param_4 * 2) == 0) {
    uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_5 * 4);
    *(undefined2 *)((int)uVar1 + param_4 * 2) = param_3;
  }
  return;
}



/* ============================================================
   1008:c452  FUN_1008_c452
   ============================================================ */

undefined2 __stdcall16far FUN_1008_c452(undefined4 param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  iVar2 = FUN_1008_91c6((int)param_1,param_1._2_2_,param_2,param_3);
  if ((((iVar2 == 0) ||
       (iVar2 = FUN_1008_91c6((int)param_1,param_1._2_2_,param_2 + 1,param_3 + 1), iVar2 == 0)) ||
      (uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4),
      *(int *)((int)uVar1 + param_2 * 2) != 0)) ||
     (((uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4 + 4),
       *(int *)((int)uVar1 + param_2 * 2) != 0 ||
       (uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4),
       *(int *)((int)uVar1 + param_2 * 2 + 2) != 0)) ||
      (uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 * 4 + 4),
      *(int *)((int)uVar1 + param_2 * 2 + 2) != 0)))) {
    uVar3 = 0;
  }
  else {
    param_3 = param_3 * 4;
    uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3);
    param_2 = param_2 * 2;
    *(undefined2 *)((int)uVar1 + param_2) = 0x7f;
    uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 + 4);
    *(undefined2 *)((int)uVar1 + param_2) = 0x80;
    uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3);
    *(undefined2 *)((int)uVar1 + param_2 + 2) = 0x81;
    uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + param_3 + 4);
    uVar3 = 1;
    *(undefined2 *)((int)uVar1 + param_2 + 2) = 0x82;
  }
  return uVar3;
}



/* ============================================================
   1008:c5b0  FUN_1008_c5b0
   ============================================================ */

void __stdcall16far
FUN_1008_c5b0(int param_1,undefined2 param_2,int param_3,int param_4,int param_5,int param_6,
             undefined2 param_7)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined2 unaff_SS;
  undefined1 local_26 [34];
  int local_4;
  
  FUN_1008_c9aa(param_1,param_2,param_3,param_4,param_5,param_6);
  for (local_4 = param_6; iVar3 = param_5, local_4 <= param_4; local_4 = local_4 + 1) {
    for (; iVar3 <= param_3; iVar3 = iVar3 + 1) {
      iVar2 = FUN_1008_91c6(param_1,param_2,iVar3,local_4);
      if (iVar2 != 0) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + local_4 * 4);
        *(undefined2 *)((int)uVar1 + iVar3 * 2) = 3;
      }
    }
  }
  FUN_1010_90bc(local_26,unaff_SS,param_1 + 0x24,param_2,param_7,param_1,param_2,param_5 + 1,
                param_6 + 1);
  FUN_1010_91a2(local_26,unaff_SS);
  FUN_1008_c662(param_1,param_2);
  FUN_1010_9360(local_26,unaff_SS);
  return;
}



/* ============================================================
   1008:c662  FUN_1008_c662
   ============================================================ */

void __stdcall16far FUN_1008_c662(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 unaff_SS;
  int local_1e [11];
  int local_8;
  int local_6;
  int local_4;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  local_6 = *(int *)(iVar4 + 0x24);
  do {
    if (*(int *)(iVar4 + 0x28) < local_6) {
      return;
    }
    for (iVar3 = *(int *)(iVar4 + 0x26); iVar3 <= *(int *)(iVar4 + 0x2a); iVar3 = iVar3 + 1) {
      iVar2 = FUN_1008_c9ce(iVar4,uVar5,iVar3,local_6);
      if (iVar2 != 0) {
        local_4 = 0;
        local_8 = iVar3;
        for (iVar2 = 0; iVar3 = local_6, iVar2 < 0xb; iVar2 = iVar2 + 1) {
          local_1e[iVar2] = 1;
        }
        iVar2 = FUN_1008_c9ce(iVar4,uVar5,local_8,local_6 + 1);
        if (iVar2 != 0) {
          local_1e[1] = 0;
          local_1e[3] = 0;
          local_1e[4] = 0;
          local_1e[7] = 0;
        }
        iVar2 = FUN_1008_c9ce(iVar4,uVar5,local_8,iVar3 + -1);
        if (iVar2 != 0) {
          local_1e[1] = 0;
          local_1e[2] = 0;
          local_1e[5] = 0;
          local_1e[6] = 0;
        }
        iVar2 = FUN_1008_c9ce(iVar4,uVar5,local_8 + 1,iVar3);
        if (iVar2 != 0) {
          local_1e[0] = 0;
          local_1e[4] = 0;
          local_1e[5] = 0;
          local_1e[8] = 0;
        }
        iVar2 = FUN_1008_c9ce(iVar4,uVar5,local_8 + -1,iVar3);
        if (iVar2 != 0) {
          local_1e[0] = 0;
          local_1e[2] = 0;
          local_1e[3] = 0;
          local_1e[9] = 0;
        }
        iVar2 = FUN_1008_c9ce(iVar4,uVar5,local_8,iVar3 + 1);
        if (iVar2 == 0) {
          local_1e[2] = 0;
          local_1e[5] = 0;
          local_1e[6] = 0;
          local_1e[8] = 0;
          local_1e[9] = 0;
          local_1e[10] = 0;
        }
        iVar2 = FUN_1008_c9ce(iVar4,uVar5,local_8,iVar3 + -1);
        if (iVar2 == 0) {
          local_1e[3] = 0;
          local_1e[4] = 0;
          local_1e[7] = 0;
          local_1e[8] = 0;
          local_1e[9] = 0;
          local_1e[10] = 0;
        }
        iVar2 = FUN_1008_c9ce(iVar4,uVar5,local_8 + 1,iVar3);
        if (iVar2 == 0) {
          local_1e[2] = 0;
          local_1e[3] = 0;
          local_1e[6] = 0;
          local_1e[7] = 0;
          local_1e[9] = 0;
          local_1e[10] = 0;
        }
        iVar3 = FUN_1008_c9ce(iVar4,uVar5,local_8 + -1,iVar3);
        if (iVar3 == 0) {
          local_1e[4] = iVar3;
          local_1e[5] = iVar3;
          local_1e[6] = iVar3;
          local_1e[7] = iVar3;
          local_1e[8] = iVar3;
          local_1e[10] = iVar3;
        }
        local_4 = 0;
        for (iVar2 = 0; iVar3 = local_8, iVar2 < 0xb; iVar2 = iVar2 + 1) {
          if (local_1e[iVar2] == 1) {
            local_4 = iVar2;
            switch(iVar2) {
            case 0:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x36;
              break;
            case 1:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x37;
              break;
            case 2:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x38;
              break;
            case 3:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x39;
              break;
            case 4:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x3b;
              break;
            case 5:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x3a;
              break;
            case 6:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x3c;
              break;
            case 7:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x3d;
              break;
            case 8:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x3e;
              break;
            case 9:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x3f;
              break;
            case 10:
              uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + local_6 * 4);
              *(undefined2 *)((int)uVar1 + local_8 * 2) = 0x40;
            }
            break;
          }
        }
      }
    }
    local_6 = local_6 + 1;
  } while( true );
}



/* ============================================================
   1008:c9aa  FUN_1008_c9aa
   ============================================================ */

void __stdcall16far
FUN_1008_c9aa(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  undefined2 unaff_CS;
  
  SETRECT(unaff_CS,param_3,param_4,param_5,param_6,param_1 + 0x24);
  return;
}



/* ============================================================
   1008:c9ce  FUN_1008_c9ce
   ============================================================ */

undefined2 __stdcall16far FUN_1008_c9ce(undefined4 param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  
  uVar4 = (undefined2)((ulong)param_1 >> 0x10);
  iVar3 = (int)param_1;
  if (((((param_3 < *(int *)(iVar3 + 0x24)) || (*(int *)(iVar3 + 0x28) < param_3)) ||
       (param_2 < *(int *)(iVar3 + 0x26))) ||
      ((*(int *)(iVar3 + 0x2a) < param_2 ||
       (iVar2 = FUN_1008_91c6(iVar3,uVar4,param_2,param_3), iVar2 == 0)))) ||
     ((uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar3 + 8) + param_3 * 4),
      *(int *)((int)uVar1 + param_2 * 2) != 3 &&
      ((uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar3 + 8) + param_3 * 4),
       *(int *)((int)uVar1 + param_2 * 2) < 0x36 ||
       (uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar3 + 8) + param_3 * 4),
       0x40 < *(int *)((int)uVar1 + param_2 * 2))))))) {
    uVar4 = 0;
  }
  else {
    uVar4 = 1;
  }
  return uVar4;
}



/* ============================================================
   1008:ca6a  FUN_1008_ca6a
   ============================================================ */

void __stdcall16far FUN_1008_ca6a(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  
  uVar4 = (undefined2)((ulong)param_1 >> 0x10);
  iVar3 = (int)param_1;
  if (0 < *(int *)(iVar3 + 0x50)) {
    iVar2 = *(int *)(iVar3 + 0x50);
    while (iVar2 = iVar2 + -1, -1 < iVar2) {
      if (*(int *)((int)*(undefined4 *)(iVar3 + 0x2c) + iVar2 * 0xc) == 3) {
        uVar5 = (undefined2)((ulong)*(undefined4 *)(iVar3 + 0x2c) >> 0x10);
        iVar1 = (int)*(undefined4 *)(iVar3 + 0x2c) + iVar2 * 0xc;
        FUN_1008_bc2c(iVar3,uVar4,*(undefined2 *)(iVar1 + 4),*(undefined2 *)(iVar1 + 2));
      }
      uVar5 = (undefined2)((ulong)*(undefined4 *)(iVar3 + 0x2c) >> 0x10);
      iVar1 = (int)*(undefined4 *)(iVar3 + 0x2c) + iVar2 * 0xc;
      FUN_1008_af5a(*(undefined2 *)(iVar3 + 0x36),*(undefined2 *)(iVar3 + 0x38),0x14,
                    *(undefined2 *)(iVar1 + 4),*(undefined2 *)(iVar1 + 2));
      FUN_1008_8032(iVar3,uVar4,iVar2);
    }
    FUN_1008_31d4(*(undefined2 *)(iVar3 + 4),*(undefined2 *)(iVar3 + 6),0xc);
  }
  return;
}



/* ============================================================
   1008:cafa  FUN_1008_cafa
   ============================================================ */

void __stdcall16far
FUN_1008_cafa(int param_1,undefined2 param_2,int param_3,int param_4,int param_5,int param_6,
             int param_7)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = FUN_1008_91c6(param_1,param_2,param_6,param_7);
  if ((iVar2 != 0) &&
     (iVar3 = FUN_1008_91c6(param_1,param_2,param_4,param_5), iVar2 = param_7, iVar3 != 0)) {
    for (; iVar3 = param_6, iVar2 <= param_5; iVar2 = iVar2 + 1) {
      if (param_3 == -1) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + iVar2 * 4);
        *(undefined2 *)((int)uVar1 + param_6 * 2) = 0x36;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + iVar2 * 4);
        *(undefined2 *)((int)uVar1 + param_4 * 2) = 0x36;
      }
      else {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + iVar2 * 4);
        *(int *)((int)uVar1 + param_6 * 2) = param_3;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + iVar2 * 4);
        *(int *)((int)uVar1 + param_4 * 2) = param_3;
      }
    }
    for (; iVar3 <= param_4; iVar3 = iVar3 + 1) {
      if (param_3 == -1) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_7 * 4);
        *(undefined2 *)((int)uVar1 + iVar3 * 2) = 0x37;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_5 * 4);
        *(undefined2 *)((int)uVar1 + iVar3 * 2) = 0x37;
      }
      else {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_7 * 4);
        *(int *)((int)uVar1 + iVar3 * 2) = param_3;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_5 * 4);
        *(int *)((int)uVar1 + iVar3 * 2) = param_3;
      }
    }
    if (param_3 == -1) {
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_7 * 4);
      *(undefined2 *)((int)uVar1 + param_6 * 2) = 0x38;
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_5 * 4);
      *(undefined2 *)((int)uVar1 + param_6 * 2) = 0x39;
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_7 * 4);
      *(undefined2 *)((int)uVar1 + param_4 * 2) = 0x3a;
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_5 * 4);
      *(undefined2 *)((int)uVar1 + param_4 * 2) = 0x3b;
    }
    else {
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_7 * 4);
      *(int *)((int)uVar1 + param_6 * 2) = param_3;
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_5 * 4);
      *(int *)((int)uVar1 + param_6 * 2) = param_3;
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_7 * 4);
      *(int *)((int)uVar1 + param_4 * 2) = param_3;
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + param_5 * 4);
      *(int *)((int)uVar1 + param_4 * 2) = param_3;
    }
  }
  return;
}



/* ============================================================
   1008:ccfe  FUN_1008_ccfe
   ============================================================ */

void __stdcall16far
FUN_1008_ccfe(int param_1,undefined2 param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  
  for (iVar1 = 0; iVar1 < *(int *)(param_1 + 0x50); iVar1 = iVar1 + 1) {
    if ((((param_6 <= *(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 2)) &&
         (*(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 2) <= param_4)) &&
        (param_5 <= *(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 4))) &&
       ((*(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 4) <= param_3 &&
        (((*(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 10) == 1 ||
          (*(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 10) == 3)) ||
         (*(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 10) == 2)))))) {
      *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 10) = 0;
    }
  }
  return;
}



/* ============================================================
   1008:cdbe  FUN_1008_cdbe
   ============================================================ */

void __stdcall16far
FUN_1008_cdbe(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,int param_5,
             int param_6,int param_7,int param_8)

{
  int iVar1;
  
  for (iVar1 = 0; iVar1 < *(int *)(param_1 + 0x50); iVar1 = iVar1 + 1) {
    if ((((param_8 <= *(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 2)) &&
         (*(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 2) <= param_6)) &&
        (param_7 <= *(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 4))) &&
       (*(int *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 4) <= param_5)) {
      *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 6) = param_4;
      *(undefined2 *)((int)*(undefined4 *)(param_1 + 0x2c) + iVar1 * 0xc + 8) = param_3;
    }
  }
  return;
}



/* ============================================================
   1008:ce4e  FUN_1008_ce4e
   ============================================================ */

undefined2 __stdcall16far
FUN_1008_ce4e(int param_1,undefined2 param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar3 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar2 = (int)*(undefined4 *)(param_1 + 4);
  iVar1 = *(int *)(iVar2 + 0xea);
  iVar2 = *(int *)(iVar2 + 0xec);
  if ((((iVar1 < param_6) || (param_4 < iVar1)) || (iVar2 < param_5)) || (param_3 < iVar2)) {
    uVar3 = 0;
  }
  else {
    uVar3 = 1;
  }
  return uVar3;
}



/* ============================================================
   1008:ce88  FUN_1008_ce88
   ============================================================ */

void __stdcall16far
FUN_1008_ce88(undefined4 param_1,undefined2 param_2,int param_3,int param_4,int param_5,int param_6)

{
  undefined4 uVar1;
  int iVar2;
  
  for (; iVar2 = param_5, param_6 <= param_4; param_6 = param_6 + 1) {
    for (; iVar2 <= param_3; iVar2 = iVar2 + 1) {
      uVar1 = *(undefined4 *)((int)param_1 + 8);
      uVar1 = *(undefined4 *)((int)uVar1 + param_6 * 4);
      *(undefined2 *)((int)uVar1 + iVar2 * 2) = param_2;
    }
  }
  return;
}



/* ============================================================
   1008:ced2  FUN_1008_ced2
   ============================================================ */

int __stdcall16far FUN_1008_ced2(void)

{
  if (5 < DAT_1080_14ce % 10) {
    return -(DAT_1080_14ce % 10 + -0xb);
  }
  return DAT_1080_14ce % 10 + 1;
}



/* ============================================================
   1008:cefc  FUN_1008_cefc
   ============================================================ */

undefined2 __stdcall16far FUN_1008_cefc(void)

{
  int iVar1;
  
  iVar1 = DAT_1080_14ce % 4;
  if (iVar1 == 0) {
    return 0xd;
  }
  if (iVar1 != 1) {
    if (iVar1 == 2) {
      return 0xf;
    }
    if (iVar1 != 3) {
      return 0xd;
    }
  }
  return 0xe;
}



/* ============================================================
   1008:cf26  FUN_1008_cf26
   ============================================================ */

undefined2 __stdcall16far FUN_1008_cf26(void)

{
  int iVar1;
  
  iVar1 = DAT_1080_14ce % 4;
  if (iVar1 != 0) {
    if (iVar1 == 1) {
      return 0xb;
    }
    if ((iVar1 != 2) && (iVar1 == 3)) {
      return 0xc;
    }
  }
  return 10;
}



/* ============================================================
   1008:cf50  FUN_1008_cf50
   ============================================================ */

int __stdcall16far
FUN_1008_cf50(undefined4 param_1,undefined2 param_2,undefined2 param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = FUN_1008_c1e6((int)param_1,param_1._2_2_,param_2,param_3,param_4,param_5);
  iVar2 = param_5;
  if (iVar1 == 1) {
    for (; iVar3 = param_4, iVar2 < param_5 + 3; iVar2 = iVar2 + 1) {
      for (; iVar3 < param_4 + 3; iVar3 = iVar3 + 1) {
        FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),0x16
                      ,iVar3,iVar2);
      }
    }
    FUN_1008_31d4(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0x16);
  }
  return iVar1;
}



/* ============================================================
   1008:d4e0  FUN_1008_d4e0
   ============================================================ */

undefined2 * __stdcall16far
FUN_1008_d4e0(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  int iVar1;
  undefined4 uVar2;
  bool bVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  undefined2 uVar15;
  int local_12;
  int local_a;
  int local_4;
  
  FUN_1008_7560((undefined2 *)param_1,param_1._2_2_,param_2,param_3,param_4,param_5);
  *param_1 = 0xe02a;
  ((undefined2 *)param_1)[1] = (char *)s_Lucky_Chip_1080_1004 + 4;
  iVar1 = *(int *)((int)*(undefined4 *)((undefined2 *)param_1 + 2) + 0xe4);
  for (iVar12 = 0; iVar12 < ((undefined2 *)param_1)[0x1e] + iVar1 + 1; iVar12 = iVar12 + 1) {
    iVar4 = FUN_1008_3c88(5);
    if (1 < iVar4) {
      uVar5 = FUN_1008_3c88(0x26);
      uVar6 = FUN_1008_3c88(0x22,uVar5);
      FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],1,uVar6,uVar5);
    }
  }
  iVar12 = 0;
  while( true ) {
    if ((int)((undefined2 *)param_1)[0x1e] < 4) {
      iVar4 = 1;
    }
    else {
      iVar4 = 2;
    }
    if (iVar4 <= iVar12) break;
    iVar4 = FUN_1008_3c88(5);
    if (5 - iVar1 < iVar4) {
      uVar5 = FUN_1008_3c88(0x25);
      uVar6 = FUN_1008_3c88(0x21,uVar5);
      FUN_1008_c452((undefined2 *)param_1,param_1._2_2_,uVar6,uVar5);
    }
    iVar4 = FUN_1008_3c88(0xc);
    if ((6 - iVar1) * 2 < iVar4) {
      uVar5 = FUN_1008_3c88(0x26);
      uVar6 = FUN_1008_3c88(0x22,uVar5);
      FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],2,uVar6,uVar5);
    }
    iVar12 = iVar12 + 1;
  }
  local_4 = ((undefined2 *)param_1)[0x1e] + iVar1;
  if ((iVar1 < 2) && (4 < local_4)) {
    local_4 = 4;
  }
  for (iVar12 = 0; iVar12 < local_4; iVar12 = iVar12 + 1) {
    iVar4 = FUN_1008_3c88(3);
    if (1 < iVar4) {
      if (((int)((undefined2 *)param_1)[0x1e] < 5) || (iVar4 = FUN_1008_3c88(0x14), iVar4 != 0x14))
      {
        uVar5 = 0x18;
      }
      else {
        uVar5 = 0x1a;
      }
      uVar6 = FUN_1008_3c88(0x24);
      uVar15 = FUN_1008_3c88(0x20,uVar6);
      FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,0,uVar5,uVar15,uVar6);
    }
  }
  uVar5 = FUN_1008_3c88(0x26);
  uVar6 = FUN_1008_3c88(0x22,uVar5);
  FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],1,uVar6,uVar5);
  iVar12 = 0;
  while( true ) {
    if ((int)((undefined2 *)param_1)[0x1e] < 5) {
      iVar4 = 1;
    }
    else {
      iVar4 = 2;
    }
    if (iVar4 <= iVar12) break;
    iVar4 = FUN_1008_3c88(iVar1 + 2);
    if (iVar4 <= iVar1) {
      uVar5 = FUN_1008_3c88(0x26);
      uVar6 = FUN_1008_3c88(0x22,uVar5);
      FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],7,uVar6,uVar5);
    }
    iVar12 = iVar12 + 1;
  }
  for (iVar12 = 0; iVar12 < (int)((undefined2 *)param_1)[0x1e] / 3 + iVar1; iVar12 = iVar12 + 1) {
    iVar4 = FUN_1008_3c88(3);
    if (1 < iVar4) {
      uVar15 = 0;
      uVar5 = FUN_1008_3c88(0x26,0);
      uVar6 = FUN_1008_3c88(0x22,uVar5);
      FUN_1008_80c0((undefined2 *)param_1,param_1._2_2_,0,0,0,1,uVar6,uVar5,uVar15);
    }
    if ((3 < (int)((undefined2 *)param_1)[0x1e]) && (iVar4 = FUN_1008_3c88(4), iVar4 == 4)) {
      uVar15 = 0;
      uVar5 = FUN_1008_3c88(0x26,0);
      uVar6 = FUN_1008_3c88(0x22,uVar5);
      FUN_1008_80c0((undefined2 *)param_1,param_1._2_2_,0,0,0,1,uVar6,uVar5,uVar15);
    }
  }
  iVar12 = FUN_1008_3c88(3);
  FUN_1008_9504((undefined2 *)param_1,param_1._2_2_,3,8,iVar12 + 1);
  iVar12 = FUN_1008_3c88(iVar1 + 2);
  if (iVar1 < iVar12) {
    local_12 = 0;
  }
  else {
    if ((int)((undefined2 *)param_1)[0x1e] < 4) {
      iVar12 = 2;
    }
    else {
      iVar12 = iVar1 / 3 + 3;
    }
    local_12 = FUN_1008_3c88(iVar12);
    local_12 = local_12 + -1;
  }
  for (iVar12 = 0; iVar12 < local_12; iVar12 = iVar12 + 1) {
    local_a = 10;
    do {
      bVar3 = true;
      iVar7 = FUN_1008_3c88(0x21);
      iVar8 = FUN_1008_3c88(0x1d);
      iVar9 = iVar8 + 1;
      iVar13 = iVar7 + 1;
      for (iVar4 = iVar13; iVar14 = iVar9, iVar4 < iVar7 + 6; iVar4 = iVar4 + 1) {
        for (; iVar14 < iVar8 + 6; iVar14 = iVar14 + 1) {
          uVar2 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar4 * 4);
          if ((*(int *)((int)uVar2 + iVar14 * 2) != 0) &&
             (uVar2 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar4 * 4),
             *(int *)((int)uVar2 + iVar14 * 2) != 3)) {
            bVar3 = false;
          }
        }
      }
      local_a = local_a + -1;
    } while ((!bVar3) && (0 < local_a));
    if (local_a == 0) break;
    FUN_1008_ce88((undefined2 *)param_1,param_1._2_2_,0,iVar8 + 5,iVar7 + 5,iVar9,iVar13);
    FUN_1008_cafa((undefined2 *)param_1,param_1._2_2_,3,iVar8 + 5,iVar7 + 5,iVar9,iVar13);
    for (iVar4 = iVar7 + 2; iVar13 = iVar9, iVar4 < iVar7 + 5; iVar4 = iVar4 + 1) {
      while (iVar13 = iVar13 + 1, iVar13 < iVar8 + 5) {
        iVar14 = FUN_1008_3c88(4);
        if (iVar14 == 4) {
          iVar14 = iVar13;
          iVar11 = iVar4;
          iVar10 = FUN_1008_3c88(5,iVar13,iVar4);
          if (iVar10 == 5) {
            uVar5 = 2;
          }
          else {
            uVar5 = 1;
          }
          FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],uVar5,iVar14,
                        iVar11);
        }
        iVar14 = FUN_1008_3c88(7);
        if (iVar14 < 2) {
          iVar14 = FUN_1008_3c88(5);
          if (iVar14 < 2) {
            uVar5 = 1;
          }
          else {
            uVar5 = 2;
          }
        }
        else {
          uVar5 = 0;
        }
        FUN_1008_80c0((undefined2 *)param_1,param_1._2_2_,0,0,0,1,iVar13,iVar4,uVar5);
      }
    }
  }
  if ((5 - iVar1 < (int)((undefined2 *)param_1)[0x1e]) && (iVar12 = FUN_1008_3c88(4), iVar12 == 4))
  {
    local_4 = 2;
  }
  else {
    local_4 = 1;
  }
  iVar12 = FUN_1008_3c88(local_4);
  for (iVar4 = 0; iVar4 < iVar12 + -1; iVar4 = iVar4 + 1) {
    local_a = 10;
    do {
      bVar3 = true;
      iVar8 = FUN_1008_3c88(0x20);
      iVar9 = FUN_1008_3c88(0x1c);
      iVar14 = iVar9 + 1;
      iVar13 = iVar8 + 1;
      for (iVar7 = iVar13; iVar11 = iVar14, iVar7 < iVar8 + 7; iVar7 = iVar7 + 1) {
        for (; iVar11 < iVar9 + 7; iVar11 = iVar11 + 1) {
          uVar2 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar7 * 4);
          if (*(int *)((int)uVar2 + iVar11 * 2) != 0) {
            bVar3 = false;
          }
        }
      }
      local_a = local_a + -1;
    } while ((!bVar3) && (0 < local_a));
    if (local_a == 0) break;
    FUN_1008_ce88((undefined2 *)param_1,param_1._2_2_,0,iVar9 + 6,iVar8 + 6,iVar14,iVar13);
    FUN_1008_cafa((undefined2 *)param_1,param_1._2_2_,3,iVar9 + 6,iVar8 + 6,iVar14,iVar13);
    FUN_1008_c452((undefined2 *)param_1,param_1._2_2_,iVar9 + 3,iVar8 + 3);
    for (local_4 = iVar8 + 2; iVar7 = iVar14, local_4 < iVar8 + 6; local_4 = local_4 + 1) {
      while (iVar7 = iVar7 + 1, iVar7 < iVar9 + 6) {
        if (((((iVar13 - local_4 == -1) || (iVar13 - local_4 == -4)) || (iVar14 - iVar7 == -1)) ||
            (iVar14 - iVar7 == -4)) && (iVar11 = FUN_1008_3c88(2), iVar11 == 2)) {
          FUN_1008_80c0((undefined2 *)param_1,param_1._2_2_,0,0,0,1,iVar7,local_4,2);
        }
      }
    }
  }
  do {
    do {
      iVar12 = FUN_1008_3c88(0x25);
      iVar4 = FUN_1008_3c88(0x21);
      uVar2 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar12 * 4);
    } while (*(int *)((int)uVar2 + iVar4 * 2) != 0);
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar12 * 4 + 8);
  } while (*(int *)((int)uVar2 + iVar4 * 2 + 4) != 0);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar12 * 4);
  *(undefined2 *)((int)uVar2 + iVar4 * 2) = 0x23;
  if (((undefined2 *)param_1)[0x1e] == 1) {
    uVar2 = *(undefined4 *)(iVar12 * 4 + (int)*(undefined4 *)((undefined2 *)param_1 + 4) + 8);
    *(undefined2 *)((int)uVar2 + iVar4 * 2 + 4) = 0x7e;
  }
  do {
    iVar12 = FUN_1008_3c88(0x26);
    iVar4 = FUN_1008_3c88(0x22);
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar12 * 4);
  } while (*(int *)((int)uVar2 + iVar4 * 2) != 0);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar12 * 4);
  *(undefined2 *)((int)uVar2 + iVar4 * 2) = 0x27;
  ((undefined2 *)param_1)[0x25] = (iVar1 + 5) * 3;
  ((undefined2 *)param_1)[0x26] = iVar1;
  ((undefined2 *)param_1)[0x27] = (iVar1 + -8) * -0xf;
  return param_1;
}



/* ============================================================
   1008:dbdc  FUN_1008_dbdc
   ============================================================ */

void __stdcall16far FUN_1008_dbdc(undefined4 param_1,int param_2)

{
  int iVar1;
  int in_DX;
  int iVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 unaff_SS;
  undefined4 uVar5;
  undefined2 *puVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined1 local_e [4];
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_4;
  
  iVar2 = (int)param_1;
  uVar7 = (undefined2)((ulong)param_1 >> 0x10);
  if (param_2 < 0x28) {
    if (*(int *)(iVar2 + 0x16) == 0 && *(int *)(iVar2 + 0x14) == 0) {
      if (((*(int *)(iVar2 + 0x3c) < 5) ||
          (iVar1 = FUN_1008_3c88(8), *(int *)(iVar2 + 0x3c) <= iVar1)) ||
         (iVar1 = FUN_1008_3a6e(), iVar1 == 0)) {
        iVar1 = FUN_1000_1646(0x54);
        if (in_DX == 0 && iVar1 == 0) goto LAB_1008_dc71;
        uVar5 = FUN_1008_d4e0(iVar1,in_DX,0,*(int *)(iVar2 + 0x3c) + 1,*(undefined2 *)(iVar2 + 4),
                              *(undefined2 *)(iVar2 + 6));
LAB_1008_dc64:
        *(undefined2 *)(iVar2 + 0x14) = (int)uVar5;
        *(undefined2 *)(iVar2 + 0x16) = (int)((ulong)uVar5 >> 0x10);
      }
      else {
        iVar1 = FUN_1000_1646(0x54);
        if (in_DX != 0 || iVar1 != 0) {
          uVar5 = FUN_1010_4044(iVar1,in_DX,0,*(int *)(iVar2 + 0x3c) + 1,*(undefined2 *)(iVar2 + 4),
                                *(undefined2 *)(iVar2 + 6));
          goto LAB_1008_dc64;
        }
LAB_1008_dc71:
        *(undefined2 *)(iVar2 + 0x16) = 0;
        *(undefined2 *)(iVar2 + 0x14) = 0;
      }
      if (*(int *)(iVar2 + 0x16) == 0 && *(int *)(iVar2 + 0x14) == 0) {
        uVar8 = 0xdfd4;
        uVar3 = *(undefined2 *)(iVar2 + 6);
        uVar7 = *(undefined2 *)(iVar2 + 4);
        goto LAB_1008_dbfb;
      }
      uVar3 = (undefined2)((ulong)*(undefined4 *)(iVar2 + 0x14) >> 0x10);
      iVar1 = (int)*(undefined4 *)(iVar2 + 0x14);
      *(int *)(iVar1 + 0xe) = iVar2;
      *(undefined2 *)(iVar1 + 0x10) = uVar7;
    }
    uVar3 = *(undefined2 *)(iVar2 + 0x14);
    uVar8 = *(undefined2 *)(iVar2 + 0x16);
    local_4 = uVar8;
    puVar6 = (undefined2 *)FUN_1008_b946(uVar3,uVar8,0,local_e,unaff_SS);
    local_a = *puVar6;
    local_8 = ((undefined2 *)puVar6)[1];
    *(undefined2 *)((int)*(undefined4 *)(iVar2 + 4) + 0xea) = local_a;
    *(undefined2 *)((int)*(undefined4 *)(iVar2 + 4) + 0xec) = local_8;
    uVar4 = (undefined2)((ulong)*(undefined4 *)(iVar2 + 4) >> 0x10);
    iVar2 = (int)*(undefined4 *)(iVar2 + 4);
    *(undefined2 *)(iVar2 + 0xdc) = uVar3;
    *(undefined2 *)(iVar2 + 0xde) = uVar8;
    FUN_1008_a5fa();
    DAT_1080_1070 = 1;
    FUN_1008_a652(uVar3,uVar8);
    REDRAWWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,0);
  }
  else {
    uVar8 = 0xdfae;
    uVar3 = *(undefined2 *)(iVar2 + 6);
    uVar7 = *(undefined2 *)(iVar2 + 4);
LAB_1008_dbfb:
    FUN_1008_2d02(uVar7,uVar3,0,uVar8,(char *)s_Lucky_Chip_1080_1004 + 4);
  }
  return;
}



/* ============================================================
   1008:dd76  FUN_1008_dd76
   ============================================================ */

void __stdcall16far
FUN_1008_dd76(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = *(int *)((int)*(undefined4 *)(param_1 + 4) + 0xe4);
  if ((*(int *)(param_1 + 0x3e) == 6) && (*(int *)(param_1 + 0x40) == 0)) {
    iVar1 = iVar6 + 5;
    iVar6 = iVar6 + *(int *)(param_1 + 0x3c) + -1;
    if (iVar1 < iVar6) {
      iVar6 = iVar1;
    }
    *(int *)(param_1 + 0x4a) = iVar6;
  }
  if ((-1 < *(int *)(param_1 + 0x40)) &&
     (((0 < *(int *)(param_1 + 0x40) || (0x32 < *(uint *)(param_1 + 0x3e))) &&
      (*(int *)(param_1 + 0x42) == 0)))) {
    *(int *)(param_1 + 0x4e) = *(int *)(param_1 + 0x4e) + -5;
    *(int *)(param_1 + 0x4a) = *(int *)(param_1 + 0x4a) + 2;
    *(undefined2 *)(param_1 + 0x42) = 1;
  }
  if (((-1 < *(int *)(param_1 + 0x40)) &&
      ((0 < *(int *)(param_1 + 0x40) || (0x96 < *(uint *)(param_1 + 0x3e))))) &&
     (*(int *)(param_1 + 0x44) == 0)) {
    *(int *)(param_1 + 0x4e) = *(int *)(param_1 + 0x4e) / 2;
    *(int *)(param_1 + 0x4a) = *(int *)(param_1 + 0x4a) << 1;
    *(undefined2 *)(param_1 + 0x44) = 1;
  }
  uVar2 = (*(int *)(param_1 + 0x3c) + 10) * 0x1e;
  iVar6 = (int)uVar2 >> 0xf;
  if (((iVar6 <= *(int *)(param_1 + 0x40)) &&
      ((iVar6 < *(int *)(param_1 + 0x40) || (uVar2 < *(uint *)(param_1 + 0x3e))))) &&
     (*(int *)(param_1 + 0x46) == 0)) {
    iVar6 = 0;
    do {
      iVar1 = FUN_1008_3c88(0x24);
      iVar3 = FUN_1008_3c88(0x20);
      iVar4 = FUN_1008_ce4e(param_1,param_2,iVar3 + 2,iVar1 + 2,iVar3,iVar1);
      if (iVar4 == 0) {
        iVar6 = FUN_1008_cf50(param_1,param_2,0,0x18,iVar3,iVar1);
      }
    } while (iVar6 == 0);
    *(undefined2 *)(param_1 + 0x4e) = 10;
    *(undefined2 *)(param_1 + 0x4a) = 0x19;
    *(undefined2 *)(param_1 + 0x46) = 1;
  }
  uVar2 = (*(int *)(param_1 + 0x3c) + 9) * 0x32;
  iVar6 = (int)uVar2 >> 0xf;
  if ((iVar6 <= *(int *)(param_1 + 0x40)) &&
     (((iVar6 < *(int *)(param_1 + 0x40) || (uVar2 < *(uint *)(param_1 + 0x3e))) &&
      (*(int *)(param_1 + 0x48) == 0)))) {
    for (iVar6 = 0; iVar6 < *(int *)(param_1 + 0x3c) + 5; iVar6 = iVar6 + 1) {
      iVar1 = 0;
      do {
        iVar3 = FUN_1008_3c88(0x24);
        iVar4 = FUN_1008_3c88(0x20);
        iVar5 = FUN_1008_ce4e(param_1,param_2,iVar4 + 2,iVar3 + 2,iVar4,iVar3);
        if (iVar5 == 0) {
          iVar1 = FUN_1008_cf50(param_1,param_2,0,0x18,iVar4,iVar3);
        }
      } while (iVar1 == 0);
    }
    *(undefined2 *)(param_1 + 0x4e) = 5;
    *(undefined2 *)(param_1 + 0x4a) = 0x28;
    *(undefined2 *)(param_1 + 0x48) = 1;
  }
  FUN_1008_ba30(param_1,param_2,param_3,param_4,param_5);
  return;
}



/* ============================================================
   1008:df52  FUN_1008_df52
   ============================================================ */

void __stdcall16far FUN_1008_df52(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar1 = *(undefined4 *)((int)param_1 + 4);
  uVar3 = (undefined2)((ulong)uVar1 >> 0x10);
  iVar2 = (int)uVar1;
  FUN_1008_67e6(*(undefined2 *)(iVar2 + 0xc),*(undefined2 *)(iVar2 + 0xe),0xdffa,
                (char *)s_Lucky_Chip_1080_1004 + 4);
  return;
}



/* ============================================================
   1008:e07a  FUN_1008_e07a
   ============================================================ */

undefined2 * __stdcall16far
FUN_1008_e07a(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  
  FUN_1008_7560(param_1,param_2,param_3,param_4,param_5,param_6);
  *param_1 = 0xe50;
  param_1[1] = 0x1010;
  iVar4 = *(int *)((int)*(undefined4 *)(param_1 + 2) + 0xe4);
  FUN_1008_b164(param_1,param_2,0,0x1038);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x30);
  *(undefined2 *)((int)uVar2 + 0x18) = 0x51;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x34);
  *(undefined2 *)((int)uVar2 + 0x18) = 0x52;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x30);
  *(undefined2 *)((int)uVar2 + 0x1a) = 0x53;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x34);
  *(undefined2 *)((int)uVar2 + 0x1a) = 0x54;
  FUN_1008_bf2a(param_1,param_2,7,7);
  FUN_1008_bf2a(param_1,param_2,7,0x10);
  FUN_1008_bf2a(param_1,param_2,0x10,7);
  FUN_1008_bf2a(param_1,param_2,0x10,0x10);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x28);
  *(undefined2 *)((int)uVar2 + 0x10) = 0x43;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x2c);
  *(undefined2 *)((int)uVar2 + 0x10) = 0x41;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x30);
  *(undefined2 *)((int)uVar2 + 0x10) = 0x43;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x34);
  *(undefined2 *)((int)uVar2 + 0x10) = 0x41;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x38);
  *(undefined2 *)((int)uVar2 + 0x10) = 0x43;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x3c);
  *(undefined2 *)((int)uVar2 + 0x10) = 0x41;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x28);
  *(undefined2 *)((int)uVar2 + 0x22) = 0x43;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x2c);
  *(undefined2 *)((int)uVar2 + 0x22) = 0x41;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x30);
  *(undefined2 *)((int)uVar2 + 0x22) = 0x43;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x34);
  *(undefined2 *)((int)uVar2 + 0x22) = 0x41;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x38);
  *(undefined2 *)((int)uVar2 + 0x22) = 0x43;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x3c);
  *(undefined2 *)((int)uVar2 + 0x22) = 0x41;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar2 + 0x14) = 0x44;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar2 + 0x16) = 0x42;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar2 + 0x18) = 0x44;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar2 + 0x1a) = 0x42;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar2 + 0x1c) = 0x44;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar2 + 0x1e) = 0x42;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x44);
  *(undefined2 *)((int)uVar2 + 0x14) = 0x44;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x44);
  *(undefined2 *)((int)uVar2 + 0x16) = 0x42;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x44);
  *(undefined2 *)((int)uVar2 + 0x18) = 0x44;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x44);
  *(undefined2 *)((int)uVar2 + 0x1a) = 0x42;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x44);
  *(undefined2 *)((int)uVar2 + 0x1c) = 0x44;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x44);
  *(undefined2 *)((int)uVar2 + 0x1e) = 0x42;
  FUN_1008_c1e6(param_1,param_2,1,0x1f,2,2);
  FUN_1008_c1e6(param_1,param_2,1,0x1f,2,0x16);
  FUN_1008_c1e6(param_1,param_2,1,0x1f,0x16,0x16);
  FUN_1008_c1e6(param_1,param_2,1,0x1f,0x16,2);
  iVar3 = 0xe;
  do {
    FUN_1008_80c0(param_1,param_2,0,1,3,1,10,iVar3,3);
    iVar3 = iVar3 + -1;
  } while (10 < iVar3);
  iVar3 = 0xe;
  do {
    FUN_1008_80c0(param_1,param_2,1,0,3,1,iVar3,0xf,3);
    iVar3 = iVar3 + -1;
  } while (10 < iVar3);
  iVar3 = 0xb;
  do {
    FUN_1008_80c0(param_1,param_2,0,0xffff,3,1,0xf,iVar3,3);
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0xf);
  iVar3 = 0xb;
  do {
    FUN_1008_80c0(param_1,param_2,0xffff,0,3,1,iVar3,10,3);
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0xf);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x2d,0x13,1);
  FUN_1008_c1e6(param_1,param_2,0,0x1d,0xe,0x20);
  uVar7 = 3;
  uVar6 = 0x21;
  uVar5 = 0xf;
  iVar3 = FUN_1008_3c88(5,0xf,0x21,3);
  iVar4 = iVar4 * 2;
  FUN_1008_80c0(param_1,param_2,0,0,2,iVar3 + iVar4,uVar5,uVar6,uVar7);
  FUN_1008_c1e6(param_1,param_2,0,0x1d,0x13,0x21);
  uVar7 = 3;
  uVar6 = 0x22;
  uVar5 = 0x14;
  iVar3 = FUN_1008_3c88(5,0x14,0x22,3);
  FUN_1008_80c0(param_1,param_2,0,0,2,iVar4 + iVar3,uVar5,uVar6,uVar7);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x38);
  *(undefined2 *)((int)uVar2 + 0x40) = 0x23;
  for (iVar4 = 0x1d; iVar4 < 0x20; iVar4 = iVar4 + 1) {
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x5c);
    *(undefined2 *)((int)uVar2 + iVar4 * 2) = 0x61;
  }
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x50);
  *(undefined2 *)((int)uVar2 + 0x38) = 0xab;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x50);
  *(undefined2 *)((int)uVar2 + 0x36) = 0xad;
  FUN_1008_c1e6(param_1,param_2,1,0x1a,0x1f,4);
  param_1[0x35] = 0;
  param_1[0x34] = 0;
  iVar4 = 0;
  do {
    param_1[iVar4 + 0x32] = 1;
    param_1[iVar4 + 0x30] = 0;
    iVar3 = iVar4 * 4;
    piVar1 = (undefined2 *)&DAT_1080_1580 + iVar4 * 2;
    param_1[iVar4 + 0x2a] = ((undefined2 *)&DAT_1080_1580)[iVar4 * 2];
    param_1[iVar4 + 0x2c] = *(undefined2 *)(iVar3 + 0x1582);
    param_1[iVar4 + 0x2e] = 0;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + *piVar1 * 4);
    *(undefined2 *)((int)uVar2 + *(int *)(iVar3 + 0x1582) * 2) = 0x90;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + *piVar1 * 4 + 4);
    *(undefined2 *)((int)uVar2 + *(int *)(iVar3 + 0x1582) * 2) = 0x91;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + *piVar1 * 4 + 8);
    *(undefined2 *)((int)uVar2 + *(int *)(iVar3 + 0x1582) * 2) = 0x92;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + *piVar1 * 4);
    *(undefined2 *)((int)uVar2 + *(int *)(iVar3 + 0x1582) * 2 + 2) = 0x93;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + *piVar1 * 4 + 4);
    *(undefined2 *)((int)uVar2 + *(int *)(iVar3 + 0x1582) * 2 + 2) = 0x94;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + *piVar1 * 4 + 8);
    *(undefined2 *)((int)uVar2 + *(int *)(iVar3 + 0x1582) * 2 + 2) = 0x95;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + *piVar1 * 4);
    *(undefined2 *)((int)uVar2 + *(int *)(iVar3 + 0x1582) * 2 + 4) = 0x96;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + *piVar1 * 4 + 4);
    *(undefined2 *)((int)uVar2 + *(int *)(iVar3 + 0x1582) * 2 + 4) = 0x97;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + *piVar1 * 4 + 8);
    iVar4 = iVar4 + 1;
    *(undefined2 *)((int)uVar2 + *(int *)(iVar3 + 0x1582) * 2 + 4) = 0x98;
  } while (iVar4 < 2);
  param_1[0x25] = 0;
  param_1[0x27] = 5;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:00ec  FUN_1010_00ec
   ============================================================ */

void __stdcall16far FUN_1010_00ec(undefined4 *param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 *puVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  int iVar13;
  undefined2 unaff_SS;
  undefined4 local_16;
  undefined4 local_12;
  uint local_a;
  int local_8;
  uint local_6;
  
  iVar1 = *(int *)((int)param_1[1] + 0xe4);
  iVar13 = 0;
  do {
    if ((*(int *)((int)param_1 + iVar13 * 2 + 100) != 0) &&
       (*(int *)((int)param_1 + iVar13 * 2 + 0x60) != 0)) {
      iVar4 = FUN_1010_0a12(param_1,param_2,iVar13);
      if (iVar4 == 1) {
        iVar4 = FUN_1008_3c88(iVar1 + 1);
        if (1 < iVar4) {
          FUN_1008_80c0(param_1,param_2,0,0,0,1,*(undefined2 *)((int)param_1 + iVar13 * 2 + 0x58),
                        *(undefined2 *)((int)param_1 + iVar13 * 2 + 0x54),2);
        }
        iVar4 = FUN_1008_3c88(iVar1 + 1);
        if (1 < iVar4) {
          FUN_1008_80c0(param_1,param_2,0,0,0,1,*(undefined2 *)((int)param_1 + iVar13 * 2 + 0x58),
                        *(int *)((int)param_1 + iVar13 * 2 + 0x54) + 2,2);
        }
        iVar4 = FUN_1008_3c88(iVar1 + 1);
        if (1 < iVar4) {
          FUN_1008_80c0(param_1,param_2,0,0,0,1,*(int *)((int)param_1 + iVar13 * 2 + 0x58) + 2,
                        *(undefined2 *)((int)param_1 + iVar13 * 2 + 0x54),2);
        }
        iVar4 = FUN_1008_3c88(iVar1 + 1);
        if (1 < iVar4) {
          uVar6 = 2;
          iVar4 = *(int *)((int)param_1 + iVar13 * 2 + 0x54) + 2;
          iVar11 = *(int *)((int)param_1 + iVar13 * 2 + 0x58) + 2;
          uVar8 = 1;
          goto LAB_1010_09a1;
        }
      }
      else {
        iVar4 = FUN_1008_3c88(0x32);
        if ((iVar4 == 0x32) ||
           ((iVar4 = FUN_1010_0a7e(param_1,param_2,iVar13), iVar4 < 2 &&
            (iVar4 = FUN_1008_3c88(iVar1 + 1), 1 < iVar4)))) {
          iVar4 = FUN_1008_3c88(2);
          if (iVar4 == 1) {
            uVar6 = 2;
          }
          else {
            uVar6 = 0;
          }
          iVar11 = iVar13 * 2;
          uVar8 = *(undefined2 *)((int)param_1 + iVar11 + 0x54);
          uVar9 = *(undefined2 *)((int)param_1 + iVar11 + 0x58);
          uVar7 = FUN_1008_3c88(1,uVar9,uVar8,uVar6);
          FUN_1008_80c0(param_1,param_2,0,0,0,uVar7,uVar9,uVar8,uVar6);
          iVar4 = FUN_1008_3c88(2);
          uVar5 = (uint)(iVar4 == 1);
          iVar4 = *(int *)((int)param_1 + iVar11 + 0x54) + 1;
          uVar6 = *(undefined2 *)((int)param_1 + iVar11 + 0x58);
          uVar8 = FUN_1008_3c88(1,uVar6,iVar4,uVar5);
          FUN_1008_80c0(param_1,param_2,0,0,0,uVar8,uVar6,iVar4,uVar5);
          iVar4 = FUN_1008_3c88(2);
          if (iVar4 == 1) {
            uVar6 = 2;
          }
          else {
            uVar6 = 0;
          }
          iVar4 = *(int *)((int)param_1 + iVar11 + 0x54) + 2;
          uVar8 = *(undefined2 *)((int)param_1 + iVar11 + 0x58);
          uVar9 = FUN_1008_3c88(1,uVar8,iVar4,uVar6);
          FUN_1008_80c0(param_1,param_2,0,0,0,uVar9,uVar8,iVar4,uVar6);
          iVar4 = FUN_1008_3c88(2);
          uVar5 = (uint)(iVar4 == 1);
          uVar6 = *(undefined2 *)((int)param_1 + iVar11 + 0x54);
          iVar4 = *(int *)((int)param_1 + iVar11 + 0x58) + 1;
          uVar8 = FUN_1008_3c88(1,iVar4,uVar6,uVar5);
          FUN_1008_80c0(param_1,param_2,0,0,0,uVar8,iVar4,uVar6,uVar5);
          iVar4 = FUN_1008_3c88(2);
          uVar5 = (uint)(iVar4 == 1);
          iVar4 = *(int *)((int)param_1 + iVar11 + 0x54) + 2;
          iVar10 = *(int *)((int)param_1 + iVar11 + 0x58) + 1;
          uVar6 = FUN_1008_3c88(1,iVar10,iVar4,uVar5);
          FUN_1008_80c0(param_1,param_2,0,0,0,uVar6,iVar10,iVar4,uVar5);
          iVar4 = FUN_1008_3c88(2);
          if (iVar4 == 1) {
            uVar6 = 2;
          }
          else {
            uVar6 = 0;
          }
          uVar8 = *(undefined2 *)((int)param_1 + iVar11 + 0x54);
          iVar4 = *(int *)((int)param_1 + iVar11 + 0x58) + 2;
          uVar9 = FUN_1008_3c88(1,iVar4,uVar8,uVar6);
          FUN_1008_80c0(param_1,param_2,0,0,0,uVar9,iVar4,uVar8,uVar6);
          iVar4 = FUN_1008_3c88(2);
          uVar5 = (uint)(iVar4 == 1);
          iVar4 = *(int *)((int)param_1 + iVar11 + 0x54) + 1;
          iVar10 = *(int *)((int)param_1 + iVar11 + 0x58) + 2;
          uVar6 = FUN_1008_3c88(1,iVar10,iVar4,uVar5);
          FUN_1008_80c0(param_1,param_2,0,0,0,uVar6,iVar10,iVar4,uVar5);
          iVar4 = FUN_1008_3c88(2);
          if (iVar4 == 1) {
            uVar6 = 2;
          }
          else {
            uVar6 = 0;
          }
          iVar4 = *(int *)((int)param_1 + iVar11 + 0x54) + 2;
          iVar11 = *(int *)((int)param_1 + iVar11 + 0x58) + 2;
          uVar8 = FUN_1008_3c88(1,iVar11,iVar4,uVar6);
LAB_1010_09a1:
          FUN_1008_80c0(param_1,param_2,0,0,0,uVar8,iVar11,iVar4,uVar6);
        }
        else {
          iVar4 = FUN_1008_3c88(3);
          if (iVar4 == 1) {
            for (local_8 = *(int *)((int)param_1 + iVar13 * 2 + 0x54);
                local_8 < *(int *)((int)param_1 + iVar13 * 2 + 0x54) + 3; local_8 = local_8 + 1) {
              for (iVar4 = *(int *)((int)param_1 + iVar13 * 2 + 0x58);
                  iVar4 < *(int *)((int)param_1 + iVar13 * 2 + 0x58) + 3; iVar4 = iVar4 + 1) {
                uVar3 = *(undefined4 *)((int)param_1[2] + local_8 * 4);
                *(undefined2 *)((int)uVar3 + iVar4 * 2) = 0;
                FUN_1008_a060(param_1,param_2,iVar4,local_8);
              }
            }
            uVar6 = (undefined2)((ulong)param_1[1] >> 0x10);
            iVar11 = (int)param_1[1];
            iVar4 = *(int *)(iVar11 + 0xea);
            iVar11 = *(int *)(iVar11 + 0xec);
            local_a = (uint)(*(int *)((int)param_1 + iVar13 * 2 + 0x54) + 1 < iVar4);
            if (iVar4 < *(int *)((int)param_1 + iVar13 * 2 + 0x54) + 1) {
              local_a = 0xffff;
            }
            local_6 = (uint)(*(int *)((int)param_1 + iVar13 * 2 + 0x58) + 1 < iVar11);
            if (iVar11 < *(int *)((int)param_1 + iVar13 * 2 + 0x58) + 1) {
              local_6 = 0xffff;
            }
            iVar4 = FUN_1010_09c0(param_1,param_2,
                                  *(int *)((int)param_1 + iVar13 * 2 + 0x58) + local_6,
                                  *(int *)((int)param_1 + iVar13 * 2 + 0x54) + local_a,iVar13);
            if (iVar4 != 0) {
              iVar4 = FUN_1010_09c0(param_1,param_2,
                                    *(int *)((int)param_1 + iVar13 * 2 + 0x58) + local_6,
                                    *(undefined2 *)((int)param_1 + iVar13 * 2 + 0x54),iVar13);
              if (iVar4 == 0) {
                local_a = 0;
              }
              else {
                iVar4 = FUN_1010_09c0(param_1,param_2,
                                      *(undefined2 *)((int)param_1 + iVar13 * 2 + 0x58),
                                      *(int *)((int)param_1 + iVar13 * 2 + 0x54) + local_a,iVar13);
                if (iVar4 != 0) {
                  local_a = 0;
                }
                local_6 = 0;
              }
            }
            iVar4 = *(int *)((int)param_1 + iVar13 * 2 + 0x54) + local_a;
            iVar11 = *(int *)((int)param_1 + iVar13 * 2 + 0x58) + local_6;
            if (iVar4 < 0) {
              local_a = 0;
            }
            if (0x25 < iVar4) {
              local_a = 0;
            }
            if (iVar11 < 0) {
              local_6 = 0;
            }
            if (0x21 < iVar11) {
              local_6 = 0;
            }
            iVar4 = iVar13 * 2;
            local_12 = (int *)CONCAT22(param_2,(int *)((int)param_1 + iVar4 + 0x54));
            *(int *)((int)param_1 + iVar4 + 0x54) = *(int *)((int)param_1 + iVar4 + 0x54) + local_a;
            *(int *)((int)param_1 + iVar4 + 0x58) = local_6 + *(int *)((int)param_1 + iVar4 + 0x58);
            for (local_8 = *local_12; local_8 < *(int *)((int)param_1 + iVar13 * 2 + 0x54) + 3;
                local_8 = local_8 + 1) {
              for (iVar4 = *(int *)((int)param_1 + iVar13 * 2 + 0x58);
                  iVar4 < *(int *)((int)param_1 + iVar13 * 2 + 0x58) + 3; iVar4 = iVar4 + 1) {
                uVar3 = *(undefined4 *)((int)param_1[2] + local_8 * 4);
                uVar5 = FUN_1008_916e(param_1,param_2,*(undefined2 *)((int)uVar3 + iVar4 * 2));
                if (((uVar5 & 2) != 0) && ((uVar5 & 4) == 0)) {
                  puVar2 = (undefined2 *)((int)*param_1 + 0x40);
                  (*(code *)*puVar2)((char *)s_Lucky_Chip_1080_1004 + 4,param_1,param_2,iVar4,
                                     local_8,3);
                }
                FUN_1008_a060(param_1,param_2,iVar4,local_8);
              }
            }
            iVar4 = iVar13 * 2;
            uVar3 = *(undefined4 *)
                     (*(int *)((int)param_1 + iVar4 + 0x54) * 4 + *(int *)(param_1 + 2));
            *(undefined2 *)((int)uVar3 + *(int *)((int)param_1 + iVar4 + 0x58) * 2) = 0x87;
            piVar12 = (int *)((int)param_1 + iVar4 + 0x58);
            uVar3 = *(undefined4 *)
                     (*(int *)((int)param_1 + iVar4 + 0x54) * 4 + *(int *)(param_1 + 2) + 4);
            *(undefined2 *)((int)uVar3 + *(int *)((int)param_1 + iVar4 + 0x58) * 2) = 0x88;
            uVar3 = *(undefined4 *)
                     (*(int *)((int)param_1 + iVar4 + 0x54) * 4 + *(int *)(param_1 + 2) + 8);
            *(undefined2 *)((int)uVar3 + *piVar12 * 2) = 0x89;
            uVar3 = *(undefined4 *)
                     (*(int *)((int)param_1 + iVar4 + 0x54) * 4 + *(int *)(param_1 + 2));
            *(undefined2 *)((int)uVar3 + *piVar12 * 2 + 2) = 0x8a;
            uVar3 = *(undefined4 *)
                     (*(int *)((int)param_1 + iVar4 + 0x54) * 4 + *(int *)(param_1 + 2) + 4);
            *(undefined2 *)((int)uVar3 + *piVar12 * 2 + 2) = 0x8b;
            uVar3 = *(undefined4 *)
                     (*(int *)((int)param_1 + iVar4 + 0x54) * 4 + *(int *)(param_1 + 2) + 8);
            *(undefined2 *)((int)uVar3 + *piVar12 * 2 + 2) = 0x8c;
            uVar3 = *(undefined4 *)
                     (*(int *)((int)param_1 + iVar4 + 0x54) * 4 + *(int *)(param_1 + 2));
            *(undefined2 *)((int)uVar3 + *piVar12 * 2 + 4) = 0x8d;
            uVar3 = *(undefined4 *)
                     (*(int *)((int)param_1 + iVar4 + 0x54) * 4 + *(int *)(param_1 + 2) + 4);
            *(undefined2 *)((int)uVar3 + *piVar12 * 2 + 4) = 0x8e;
            uVar3 = *(undefined4 *)
                     (*(int *)((int)param_1 + iVar4 + 0x54) * 4 + *(int *)(param_1 + 2) + 8);
            *(undefined2 *)((int)uVar3 + *piVar12 * 2 + 4) = 0x8f;
            for (local_6 = 0; (int)local_6 < *(int *)((int)param_1 + 0x52); local_6 = local_6 + 1) {
              uVar6 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x32) >> 0x10);
              piVar12 = (int *)((int)*(undefined4 *)((int)param_1 + 0x32) + local_6 * 10);
              iVar4 = piVar12[1];
              iVar11 = piVar12[2];
              iVar10 = *piVar12;
              if ((((iVar4 < *(int *)((int)param_1 + iVar13 * 2 + 0x54)) ||
                   (*(int *)((int)param_1 + iVar13 * 2 + 0x54) + 2 < iVar4)) ||
                  (iVar11 < *(int *)((int)param_1 + iVar13 * 2 + 0x58))) ||
                 (*(int *)((int)param_1 + iVar13 * 2 + 0x58) + 2 < iVar11)) {
                if ((((*(int *)((int)param_1 + iVar13 * 2 + 0x54) + -1 <= iVar4) &&
                     (iVar4 <= *(int *)((int)param_1 + iVar13 * 2 + 0x54) + 3)) &&
                    ((*(int *)((int)param_1 + iVar13 * 2 + 0x58) + -1 <= iVar11 &&
                     ((iVar11 <= *(int *)((int)param_1 + iVar13 * 2 + 0x58) + 3 && (iVar10 == 3)))))
                    ) && (iVar10 = FUN_1008_3c88(iVar1 * 2), 1 < iVar10)) {
                  FUN_1008_7f8e(param_1,param_2,local_6);
                  local_6 = local_6 + -1;
                  iVar10 = FUN_1008_91c6(param_1,param_2,iVar11,iVar4);
                  if (iVar10 != 0) {
                    FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),
                                  *(undefined2 *)(param_1 + 0xe),0x14,iVar11,iVar4);
                  }
                }
              }
              else {
                FUN_1008_7f8e(param_1,param_2,local_6);
                local_6 = local_6 + -1;
                if (iVar10 == 2) {
                  FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)(param_1 + 0xe),
                                0x14,iVar11,iVar4);
                  iVar4 = iVar13 * 2;
                  local_16 = (int *)CONCAT22(param_2,(int *)((int)param_1 + iVar4 + 0x5c));
                  *(int *)((int)param_1 + iVar4 + 0x5c) = *(int *)((int)param_1 + iVar4 + 0x5c) + 1;
                  if (*local_16 < 10) {
                    FUN_1008_2840(*(undefined2 *)(param_1 + 1),*(undefined2 *)((int)param_1 + 6),
                                  0x9c4,0);
                  }
                  FUN_1008_31d4(*(undefined2 *)(param_1 + 1),*(undefined2 *)((int)param_1 + 6),0xc);
                }
              }
            }
          }
        }
      }
    }
    iVar13 = iVar13 + 1;
    if (1 < iVar13) {
      return;
    }
  } while( true );
}



/* ============================================================
   1010:09c0  FUN_1010_09c0
   ============================================================ */

undefined2 __stdcall16far FUN_1010_09c0(undefined4 param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  undefined2 uVar2;
  
  uVar2 = (undefined2)((ulong)param_1 >> 0x10);
  iVar1 = FUN_1000_1f42(*(int *)((int)param_1 + (uint)(param_4 == 0) * 2 + 0x54) - param_3);
  if ((iVar1 < 3) &&
     (iVar1 = FUN_1000_1f42(*(int *)((int)param_1 + 0x58 + (uint)(param_4 == 0) * 2) - param_2),
     iVar1 < 3)) {
    return 1;
  }
  return 0;
}



/* ============================================================
   1010:0a12  FUN_1010_0a12
   ============================================================ */

int __stdcall16far FUN_1010_0a12(undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  iVar3 = (int)param_1;
  uVar6 = (undefined2)((ulong)*(undefined4 *)(iVar3 + 4) >> 0x10);
  iVar4 = (int)*(undefined4 *)(iVar3 + 4);
  iVar2 = *(int *)(iVar4 + 0xec);
  iVar1 = FUN_1000_1f42((*(int *)(iVar4 + 0xea) - *(int *)(iVar3 + 0x54 + param_2 * 2)) + -1);
  iVar4 = 0;
  if (0 < iVar1) {
    iVar4 = iVar1;
  }
  iVar2 = FUN_1000_1f42((iVar2 - *(int *)(iVar3 + 0x58 + param_2 * 2)) + -1);
  if (iVar4 < iVar2) {
    iVar4 = iVar2;
  }
  return iVar4 + -1;
}



/* ============================================================
   1010:0a7e  FUN_1010_0a7e
   ============================================================ */

int __stdcall16far FUN_1010_0a7e(int param_1,undefined2 param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int local_8;
  int local_4;
  
  local_8 = 1000;
  for (iVar2 = 0; iVar2 < *(int *)(param_1 + 0x52); iVar2 = iVar2 + 1) {
    local_4 = 0;
    if (*(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar2 * 10) == 2) {
      iVar1 = FUN_1000_1f42((*(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar2 * 10 + 2) -
                            *(int *)(param_1 + 0x54 + param_3 * 2)) + -1);
      if (0 < iVar1) {
        local_4 = iVar1;
      }
      iVar1 = FUN_1000_1f42((*(int *)((int)*(undefined4 *)(param_1 + 0x32) + iVar2 * 10 + 4) -
                            *(int *)(param_1 + 0x58 + param_3 * 2)) + -1);
      if (local_4 < iVar1) {
        local_4 = iVar1;
      }
      if (local_4 < local_8) {
        local_8 = local_4;
      }
    }
  }
  return local_8 + -1;
}



/* ============================================================
   1010:0d32  FUN_1010_0d32
   ============================================================ */

void __stdcall16far FUN_1010_0d32(undefined4 param_1)

{
  *(undefined2 *)((int)param_1 + 0xc) = 1;
  return;
}



/* ============================================================
   1010:0ea0  FUN_1010_0ea0
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_0ea0(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  FUN_1008_7560(param_1,param_2,param_3,param_4,param_5,param_6);
  *param_1 = 0x3364;
  param_1[1] = 0x1010;
  param_1[0x72] = 0;
  param_1[0x71] = 0xffff;
  param_1[0x73] = 7;
  FUN_1008_b164(param_1,param_2,0,0x1040);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0x12,0xe);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0x13,0xe);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0x14,0xe);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0x12,0x22);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0x13,0x22);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0x14,0x22);
  FUN_1008_c1e6(param_1,param_2,0,0x1e,0x1b,0x23);
  FUN_1008_c1e6(param_1,param_2,0,0x1e,0x20,3);
  FUN_1008_bf2a(param_1,param_2,2,7);
  FUN_1008_bf2a(param_1,param_2,2,0x20);
  iVar4 = 0xb;
  do {
    FUN_1008_c452(param_1,param_2,0,iVar4);
    iVar4 = iVar4 + 3;
  } while (iVar4 < 0x1e);
  iVar4 = 0xd;
  do {
    FUN_1008_c1e6(param_1,param_2,0,0x1f,5,iVar4);
    iVar4 = iVar4 + 7;
  } while (iVar4 < 0x1c);
  param_1[0x25] = 4;
  param_1[0x27] = 0x14;
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x26,0x1a,0x17);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x27,0x1a,0x18);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x28,0x1b,0x17);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x29,0x1b,0x18);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x2c,0x1a,0x1a);
  iVar4 = 0;
  do {
    iVar1 = iVar4 + 99;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + iVar4 * 0xc + 0x44);
    iVar3 = iVar4 * -2;
    iVar4 = iVar4 + 1;
    *(int *)((int)uVar2 + iVar3 + 0x46) = iVar1;
  } while (iVar4 < 6);
  iVar4 = 0;
  do {
    iVar3 = FUN_1008_3c88(4);
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + iVar4 * 0xc + 0x50);
    iVar1 = iVar4 * -2;
    iVar4 = iVar4 + 1;
    *(int *)((int)uVar2 + iVar1 + 0x46) = iVar3 + 0x68;
  } while (iVar4 < 5);
  iVar4 = FUN_1008_3c88(4);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x50);
  *(int *)((int)uVar2 + 0x36) = iVar4 + 0x68;
  iVar4 = FUN_1008_3c88(6);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x50);
  *(int *)((int)uVar2 + 0x34) = iVar4 + 0x62;
  iVar4 = FUN_1008_3c88(4);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x70);
  *(int *)((int)uVar2 + 0x36) = iVar4 + 0x68;
  iVar4 = FUN_1008_3c88(6);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x70);
  *(int *)((int)uVar2 + 0x34) = iVar4 + 0x62;
  FUN_1010_1c1e(param_1,param_2);
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:1c1e  FUN_1010_1c1e
   ============================================================ */

void __stdcall16far FUN_1010_1c1e(int param_1,undefined2 param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = FUN_1008_3c88(2);
  if (iVar3 == 2) {
    iVar3 = FUN_1008_3c88(2);
    iVar3 = iVar3 * 5 + -3;
  }
  else {
    iVar3 = 0;
  }
  *(int *)(param_1 + 0xe0) = iVar3;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0xc);
  if (*(int *)((int)uVar1 + 8) != 0) {
    for (iVar3 = 0x16; iVar3 < 0x1b; iVar3 = iVar3 + 1) {
      FUN_1008_a060(param_1,param_2,0x10,iVar3);
    }
  }
  *(undefined2 *)(param_1 + 0xdc) = 0;
  *(undefined2 *)(param_1 + 0xda) = 0;
  FUN_1010_2874(param_1,param_2,0xffff,0,0x3208,0x1010);
  iVar3 = 0;
  do {
    iVar2 = iVar3 * 2;
    iVar3 = iVar3 + 1;
    *(undefined2 *)(iVar2 + param_1 + 0xd0) = 0;
  } while (iVar3 < 5);
  FUN_1010_282c(param_1,param_2);
  return;
}



/* ============================================================
   1010:20c2  FUN_1010_20c2
   ============================================================ */

int __stdcall16far FUN_1010_20c2(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined2 unaff_SS;
  int local_28 [13];
  undefined2 local_e;
  int local_c;
  int local_a;
  int local_8;
  int local_6;
  undefined2 local_4;
  
  local_c = 1;
  iVar4 = 0;
  do {
    local_28[iVar4] = 0;
    iVar4 = iVar4 + 1;
  } while (iVar4 < 0xe);
  iVar4 = 0;
  for (iVar5 = 0; iVar5 < 5; iVar5 = iVar5 + 1) {
    if (*(int *)(param_1 + 0xbc + iVar5 * 2) == *(int *)(param_1 + 0xe0)) {
      iVar4 = iVar4 + 1;
    }
    else {
      iVar2 = *(int *)(iVar5 * 2 + param_1 + 0xbc);
      *(undefined2 *)(&stack0xffd6 + iVar2 * 2) = 1;
      if (iVar2 == 1) {
        local_e = 1;
      }
    }
  }
  local_a = iVar4;
  local_8 = -1;
  for (iVar5 = 0; (iVar2 = local_8, iVar5 < 0xd && (iVar2 = iVar5, local_28[iVar5] != 1));
      iVar5 = iVar5 + 1) {
  }
  local_8 = iVar2;
  iVar3 = iVar4;
  iVar1 = local_c;
  iVar5 = iVar2;
  while (iVar5 = iVar5 + 1, iVar5 <= iVar2 + 4) {
    if ((0xc < iVar5) || (local_28[iVar5] == 0)) {
      if (iVar3 < 1) {
        iVar1 = 0;
      }
      else {
        iVar3 = iVar3 + -1;
      }
    }
  }
  if (iVar2 == 0) {
    if (iVar1 == 1) {
      iVar1 = 1;
    }
    else {
      iVar1 = 1;
      local_4 = 1;
      local_c = 1;
      local_8 = 0xffff;
      local_6 = iVar4;
      for (iVar5 = 1; (local_8 = -1, iVar5 < 0xe && (local_8 = iVar5, local_28[iVar5] != 1));
          iVar5 = iVar5 + 1) {
      }
      iVar5 = local_8;
      while (iVar5 = iVar5 + 1, iVar5 <= local_8 + 4) {
        if ((0xd < iVar5) || (local_28[iVar5] == 0)) {
          if (iVar4 < 1) {
            iVar1 = 0;
          }
          else {
            iVar4 = iVar4 + -1;
          }
        }
      }
    }
  }
  return iVar1;
}



/* ============================================================
   1010:21f0  FUN_1010_21f0
   ============================================================ */

undefined2 __stdcall16far FUN_1010_21f0(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = -1;
  iVar2 = 0;
  do {
    if (4 < iVar2) {
      return 1;
    }
    if (*(int *)(param_1 + 0xbc + iVar2 * 2) != *(int *)(param_1 + 0xe0)) {
      if (iVar1 == -1) {
        iVar1 = *(int *)(iVar2 * 2 + param_1 + 0xc6);
      }
      else if (*(int *)(iVar2 * 2 + param_1 + 0xc6) != iVar1) {
        return 0;
      }
    }
    iVar2 = iVar2 + 1;
  } while( true );
}



/* ============================================================
   1010:224a  FUN_1010_224a
   ============================================================ */

undefined2 __stdcall16far FUN_1010_224a(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_1010_20c2(param_1,param_2);
  if ((iVar1 != 0) && (iVar1 = FUN_1010_21f0(param_1,param_2), iVar1 != 0)) {
    iVar1 = 0xff;
    iVar2 = 0;
    do {
      if (((*(int *)(iVar2 * 2 + param_1 + 0xbc) < iVar1) &&
          (1 < *(int *)(iVar2 * 2 + param_1 + 0xbc))) &&
         (*(int *)(param_1 + 0xbc + iVar2 * 2) != *(int *)(param_1 + 0xe0))) {
        iVar1 = *(int *)(iVar2 * 2 + param_1 + 0xbc);
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < 5);
    if (9 < iVar1) {
      return 1;
    }
  }
  return 0;
}



/* ============================================================
   1010:22c6  FUN_1010_22c6
   ============================================================ */

void __stdcall16far
FUN_1010_22c6(int param_1,undefined2 param_2,int *param_3,undefined2 param_4,undefined2 *param_5)

{
  int iVar1;
  
  if (((*(int *)(param_1 + 0xe4) != 3) && (9 < *(int *)((int)*(undefined4 *)(param_1 + 4) + 0xea)))
     && (*(int *)((int)*(undefined4 *)(param_1 + 4) + 0xea) < 0x20)) {
    if ((*(int *)((int)*(undefined4 *)(param_1 + 4) + 0xec) + *param_3 == 3) && (*param_3 < 0)) {
      FUN_1008_2822(*(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6),0x32c6,0x1010);
      *param_3 = 0;
      iVar1 = FUN_1008_27b8(*(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6),0,*param_5,0);
      if (iVar1 == 0) {
        *param_5 = 0;
      }
    }
  }
  return;
}



/* ============================================================
   1010:235c  FUN_1010_235c
   ============================================================ */

void __stdcall16far FUN_1010_235c(int param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  
  uVar2 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar1 = (int)*(undefined4 *)(param_1 + 4);
  if ((*(int *)(iVar1 + 0xea) == 0xc) && (*(int *)(iVar1 + 0xec) == 0x17)) {
    uVar2 = 0x32f4;
  }
  else {
    uVar2 = 0x3330;
  }
  uVar3 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar1 = (int)*(undefined4 *)(param_1 + 4);
  FUN_1008_67e6(*(undefined2 *)(iVar1 + 0xc),*(undefined2 *)(iVar1 + 0xe),uVar2,0x1010);
  return;
}



/* ============================================================
   1010:23ce  FUN_1010_23ce
   ============================================================ */

void __stdcall16far
FUN_1010_23ce(undefined4 param_1,undefined2 param_2,undefined2 param_3,int param_4,int param_5)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined2 uVar6;
  int local_4;
  
  iVar4 = (int)param_1;
  uVar6 = (undefined2)((ulong)param_1 >> 0x10);
  if (((param_4 != 0x10) || (param_5 < 0x16)) || (0x1a < param_5)) {
    if (param_4 != 0x11) {
      return;
    }
    for (iVar2 = 0; iVar2 < 5; iVar2 = iVar2 + 1) {
      if (iVar2 * 3 - param_5 == -0x11) {
        for (iVar5 = 0; iVar5 < 3; iVar5 = iVar5 + 1) {
          if (((*(int *)(iVar4 + 0xda) == 2) || (*(int *)(iVar4 + 0xda) == 3)) ||
             (*(int *)(iVar4 + 0xda) == 4)) {
            iVar3 = iVar2 * 2 + iVar4;
            FUN_1008_3bd0(param_2,param_3,0x11,param_5 + iVar5,
                          (-(uint)(*(int *)(iVar3 + 0xd0) != 0) & 8) + iVar5 + 0x11a);
            FUN_1008_3bd0(param_2,param_3,0x14,param_5 + iVar5,
                          (-(uint)(*(int *)(iVar3 + 0xd0) != 0) & 8) + iVar5 + 0x11f);
          }
          if ((*(int *)(iVar4 + 0xda) == 0) || (*(int *)(iVar4 + 0xda) == 1)) {
            FUN_1008_3bd0(param_2,param_3,0x11,param_5 + iVar5,iVar5 + 0x12a);
            FUN_1008_3bd0(param_2,param_3,0x14,param_5 + iVar5,iVar5 + 0x130);
          }
        }
        for (iVar5 = 1; iVar5 < 3; iVar5 = iVar5 + 1) {
          if (((*(int *)(iVar4 + 0xda) == 2) || (*(int *)(iVar4 + 0xda) == 3)) ||
             (*(int *)(iVar4 + 0xda) == 4)) {
            iVar3 = iVar2 * 2 + iVar4;
            FUN_1008_3bd0(param_2,param_3,iVar5 + 0x11,param_5,
                          (-(uint)(*(int *)(iVar3 + 0xd0) != 0) & 8) + 0x11d);
            FUN_1008_3bd0(param_2,param_3,iVar5 + 0x11,param_5 + 2,
                          (-(uint)(*(int *)(iVar3 + 0xd0) != 0) & 8) + 0x11e);
            if (iVar5 == 1) {
              if ((*(int *)(iVar2 * 2 + iVar4 + 0xc6) == 2) ||
                 (*(int *)(iVar2 * 2 + iVar4 + 0xc6) == 3)) {
                local_4 = 0xd;
              }
              else {
                local_4 = 0;
              }
              iVar3 = *(int *)(iVar2 * 2 + iVar4 + 0xbc) + local_4 + 0xff;
            }
            else {
              iVar3 = *(int *)(iVar2 * 2 + iVar4 + 0xc6) + 0xeb;
            }
            FUN_1008_3bd0(param_2,param_3,iVar5 + 0x11,param_5 + 1,iVar3);
          }
          if ((*(int *)(iVar4 + 0xda) == 0) || (*(int *)(iVar4 + 0xda) == 1)) {
            iVar3 = iVar5 + 0x11;
            FUN_1008_3bd0(param_2,param_3,iVar3,param_5,0x12d);
            FUN_1008_3bd0(param_2,param_3,iVar3,param_5 + 1,0x12e);
            FUN_1008_3bd0(param_2,param_3,iVar3,param_5 + 2,0x12f);
          }
        }
      }
    }
    return;
  }
  if (*(int *)(iVar4 + 0xe0) == 0) {
    uVar6 = 0xf0;
  }
  else {
    if (((*(int *)(iVar4 + 0xe0) == 2) || (*(int *)(iVar4 + 0xe0) == 7)) && (0x17 < param_5)) {
      FUN_1008_3bd0(param_2,param_3,0x10,param_5,param_5 + 0x11f);
    }
    if (0x17 < param_5) {
      return;
    }
    if (*(int *)(iVar4 + 0xe0) == 2) {
      if (param_5 == 0x16) {
        uVar1 = 0x134;
      }
      else {
        uVar1 = 0x135;
      }
      FUN_1008_3bd0(param_2,param_3,0x10,param_5,uVar1);
    }
    if (*(int *)(iVar4 + 0xe0) != 7) {
      return;
    }
    if (param_5 == 0x16) {
      uVar6 = 0x136;
    }
    else {
      uVar6 = 0x135;
    }
  }
  FUN_1008_3bd0(param_2,param_3,0x10,param_5,uVar6);
  return;
}



/* ============================================================
   1010:26fe  FUN_1010_26fe
   ============================================================ */

void __stdcall16far FUN_1010_26fe(int param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined2 *puVar7;
  undefined2 *puVar8;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar9;
  undefined2 uVar10;
  undefined2 uVar11;
  undefined2 local_18;
  int local_8;
  undefined2 local_6;
  undefined2 local_4;
  
  local_8 = param_1 + 0x54;
  local_6 = param_2;
  if ((*(int *)(param_1 + 0xda) == 3) &&
     (iVar6 = DAT_1080_1632 + 1, bVar3 = 100 < DAT_1080_1632, DAT_1080_1632 = iVar6, bVar3)) {
    unaff_CS = 0x1010;
    FUN_1010_1c1e(param_1,param_2);
    DAT_1080_1632 = 0;
  }
  puVar8 = &local_18;
  puVar7 = (undefined2 *)0x3354;
  for (iVar6 = 8; iVar5 = local_8, iVar6 != 0; iVar6 = iVar6 + -1) {
    puVar2 = puVar8;
    puVar8 = puVar8 + 1;
    puVar1 = puVar7;
    puVar7 = puVar7 + 1;
    *puVar2 = *puVar1;
  }
  local_4 = 0;
  iVar6 = 0;
  do {
    if (0xc < iVar6) {
      local_4 = SELECTOBJECT(unaff_CS,*(undefined2 *)((int)*(undefined4 *)(param_1 + 4) + 0xd6));
      uVar11 = param_3;
      SETTEXTCOLOR(0x11d0,*(undefined2 *)(param_1 + 0xb8),*(undefined2 *)(param_1 + 0xba));
      uVar10 = param_3;
      SETBKMODE(0x11d0,2);
      uVar9 = param_3;
      SETBKCOLOR(0x11d0,0,0);
      TEXTOUT(0x11d0,0xd,&local_18,unaff_SS,0x9c,0xd8);
      iVar6 = DAT_1080_1634;
      DAT_1080_1634 = DAT_1080_1634 + 1;
      iVar5 = FUN_1000_1710(local_8,local_6,param_3,uVar9,uVar10,uVar11);
      if (iVar5 + 0xd < iVar6) {
        DAT_1080_1634 = 0;
      }
      SELECTOBJECT(0x1000,local_4);
      return;
    }
    if (DAT_1080_1634 + iVar6 < 0xd) {
LAB_1010_2791:
      *(undefined1 *)((int)&local_18 + iVar6 + 1) = 0x20;
    }
    else {
      unaff_CS = 0x1000;
      iVar4 = FUN_1000_1710(iVar5,local_6);
      if (iVar4 <= DAT_1080_1634 + iVar6 + -0xd) goto LAB_1010_2791;
      *(undefined1 *)((int)&local_18 + iVar6 + 1) =
           *(undefined1 *)(DAT_1080_1634 + iVar5 + iVar6 + -0xd);
    }
    iVar6 = iVar6 + 1;
  } while( true );
}



/* ============================================================
   1010:282c  FUN_1010_282c
   ============================================================ */

void __stdcall16far FUN_1010_282c(int param_1,undefined2 param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0xc);
  if (*(int *)((int)uVar1 + 8) != 0) {
    for (iVar2 = 0x11; iVar2 < 0x20; iVar2 = iVar2 + 1) {
      iVar3 = 0x11;
      do {
        FUN_1008_a060(param_1,param_2,iVar3,iVar2);
        iVar3 = iVar3 + 1;
      } while (iVar3 < 0x16);
    }
  }
  return;
}



/* ============================================================
   1010:2874  FUN_1010_2874
   ============================================================ */

void __stdcall16far
FUN_1010_2874(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  *(undefined2 *)(param_1 + 0xb8) = param_3;
  *(undefined2 *)(param_1 + 0xba) = param_4;
  FUN_1000_16aa(param_1 + 0x54,param_2,param_5,param_6);
  DAT_1080_1634 = 0;
  return;
}



/* ============================================================
   1010:28ac  FUN_1010_28ac
   ============================================================ */

void __stdcall16far FUN_1010_28ac(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  int local_4;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  if (*(int *)(iVar4 + 0xe2) == -1) {
    DAT_1080_1636 = param_2;
  }
  iVar3 = *(int *)(iVar4 + 0xe2);
  if (iVar3 == -1) {
    iVar3 = FUN_1008_3c88(2);
    if (iVar3 == 2) {
      uVar2 = 0;
    }
    else {
      uVar2 = 100;
    }
    *(undefined2 *)(iVar4 + 0xe2) = uVar2;
    return;
  }
  if (iVar3 == 0) {
    uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + 0x10);
    if (*(int *)((int)uVar1 + 0x42) != 0x1e) goto LAB_1010_2a58;
    if (DAT_1080_1636 == 1) {
      uVar8 = 0;
      uVar7 = 4;
      uVar6 = 0x21;
      uVar2 = FUN_1008_3c88(3,0x21,4,0);
      FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
      uVar6 = 4;
      uVar2 = 0x21;
LAB_1010_2ae5:
      uVar7 = 1;
      uVar8 = 3;
    }
    else {
      if (DAT_1080_1636 == 2) {
        uVar8 = 0;
        uVar7 = 4;
        uVar6 = 0x21;
        uVar2 = FUN_1008_3c88(6,0x21,4,0);
        FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
        uVar8 = 1;
        uVar7 = 4;
        uVar6 = 0x21;
        uVar2 = FUN_1008_3c88(6,0x21,4,1);
        FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
        uVar7 = 2;
        uVar6 = 4;
        uVar2 = 0x21;
      }
      else {
        if (DAT_1080_1636 != 3) goto LAB_1010_2a58;
        uVar8 = 0;
        uVar7 = 4;
        uVar6 = 0x21;
        uVar2 = FUN_1008_3c88(6,0x21,4,0);
        FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
        uVar8 = 1;
        uVar7 = 4;
        uVar6 = 0x21;
        uVar2 = FUN_1008_3c88(6,0x21,4,1);
        FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
        uVar7 = 2;
        uVar6 = 4;
        uVar2 = 0x21;
        iVar3 = FUN_1008_3c88(6,0x21,4,2);
        FUN_1008_80c0(iVar4,uVar5,0,0,0,iVar3 + 6,uVar2,uVar6,uVar7);
        iVar3 = FUN_1008_3c88(3);
        if (iVar3 != 3) goto LAB_1010_2a58;
        uVar6 = 0x24;
        uVar2 = 0x1e;
        uVar7 = 3;
      }
LAB_1010_2a3d:
      uVar8 = 2;
    }
    uVar8 = FUN_1008_3c88(uVar8,uVar2,uVar6,uVar7);
    FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar8,uVar2,uVar6,uVar7);
    goto LAB_1010_2a58;
  }
  if (iVar3 == 1) {
    FUN_1008_cafa(iVar4,uVar5,0,0x23,6,0x1f,2);
    local_4 = 2;
    do {
      iVar3 = 0x1f;
      do {
        FUN_1008_a060(iVar4,uVar5,iVar3,local_4);
        iVar3 = iVar3 + 1;
      } while (iVar3 < 0x24);
      local_4 = local_4 + 1;
    } while (local_4 < 7);
    goto LAB_1010_2a58;
  }
  if (iVar3 + -1 < 1) {
    return;
  }
  if (SBORROW2(iVar3 + -1,1)) {
    return;
  }
  if (iVar3 == 3 || iVar3 + -2 < 1) goto LAB_1010_2a58;
  if (iVar3 == 4) {
    if (1 < DAT_1080_1636) goto LAB_1010_2a58;
LAB_1010_2bee:
    if (DAT_1080_1636 == 3) {
LAB_1010_2a58:
      *(int *)(iVar4 + 0xe2) = *(int *)(iVar4 + 0xe2) + 1;
      return;
    }
  }
  else {
    if (iVar3 == 5) goto LAB_1010_2bee;
    if (iVar3 != 6) {
      if (iVar3 == 100) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + 0x90);
        if (*(int *)((int)uVar1 + 0x38) != 0x1e) goto LAB_1010_2a58;
        if (DAT_1080_1636 == 1) {
          uVar8 = 0;
          uVar7 = 0x24;
          uVar6 = 0x1c;
          uVar2 = FUN_1008_3c88(3,0x1c,0x24,0);
          FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
          uVar6 = 0x24;
          uVar2 = 0x1c;
          goto LAB_1010_2ae5;
        }
        if (DAT_1080_1636 == 2) {
          uVar8 = 0;
          uVar7 = 0x24;
          uVar6 = 0x1c;
          uVar2 = FUN_1008_3c88(6,0x1c,0x24,0);
          FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
          uVar8 = 1;
          uVar7 = 0x24;
          uVar6 = 0x1c;
          uVar2 = FUN_1008_3c88(6,0x1c,0x24,1);
          FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
          uVar7 = 2;
        }
        else {
          if (DAT_1080_1636 != 3) goto LAB_1010_2a58;
          uVar8 = 0;
          uVar7 = 0x24;
          uVar6 = 0x1c;
          uVar2 = FUN_1008_3c88(6,0x1c,0x24,0);
          FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
          uVar8 = 1;
          uVar7 = 0x24;
          uVar6 = 0x1c;
          uVar2 = FUN_1008_3c88(6,0x1c,0x24,1);
          FUN_1008_80c0(iVar4,uVar5,0,0,0,uVar2,uVar6,uVar7,uVar8);
          uVar7 = 2;
          uVar6 = 0x24;
          uVar2 = 0x1c;
          iVar3 = FUN_1008_3c88(6,0x1c,0x24,2);
          FUN_1008_80c0(iVar4,uVar5,0,0,0,iVar3 + 6,uVar2,uVar6,uVar7);
          iVar3 = FUN_1008_3c88(3);
          if (iVar3 != 3) goto LAB_1010_2a58;
          uVar7 = 3;
        }
        uVar6 = 0x24;
        uVar2 = 0x1c;
        goto LAB_1010_2a3d;
      }
      if (iVar3 == 0x65) {
        FUN_1008_cafa(iVar4,uVar5,0,0x1e,0x26,0x1a,0x22);
        local_4 = 0x22;
        do {
          for (iVar3 = 0x1a; iVar3 < 0x1f; iVar3 = iVar3 + 1) {
            FUN_1008_a060(iVar4,uVar5,iVar3,local_4);
          }
          local_4 = local_4 + 1;
        } while (local_4 < 0x27);
        *(int *)(iVar4 + 0xe2) = *(int *)(iVar4 + 0xe2) + 1;
        return;
      }
      if (iVar3 + -0x65 < 1) {
        return;
      }
      if (iVar3 == 0x67 || iVar3 + -0x66 < 1) goto LAB_1010_2a58;
      if (iVar3 == 0x68) {
        if (1 < DAT_1080_1636) goto LAB_1010_2a58;
LAB_1010_2c45:
        if (DAT_1080_1636 == 3) goto LAB_1010_2a58;
      }
      else {
        if (iVar3 == 0x69) goto LAB_1010_2c45;
        if (iVar3 != 0x6a) {
          return;
        }
      }
      FUN_1008_cafa(iVar4,uVar5,0xffff,0x1e,0x26,0x1a,0x22);
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + 0x90);
      *(undefined2 *)((int)uVar1 + 0x3c) = 0x3f;
      local_4 = 0x22;
      do {
        for (iVar3 = 0x1a; iVar3 < 0x1f; iVar3 = iVar3 + 1) {
          FUN_1008_a060(iVar4,uVar5,iVar3,local_4);
        }
        local_4 = local_4 + 1;
      } while (local_4 < 0x27);
      goto LAB_1010_2c9b;
    }
  }
  FUN_1008_cafa(iVar4,uVar5,0xffff,0x23,6,0x1f,2);
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar4 + 8) + 0x10);
  *(undefined2 *)((int)uVar1 + 0x3e) = 0x3e;
  local_4 = 2;
  do {
    for (iVar3 = 0x1f; iVar3 < 0x24; iVar3 = iVar3 + 1) {
      FUN_1008_a060(iVar4,uVar5,iVar3,local_4);
    }
    local_4 = local_4 + 1;
  } while (local_4 < 7);
LAB_1010_2c9b:
  *(undefined2 *)(iVar4 + 0xe2) = 0xffff;
  return;
}



/* ============================================================
   1010:2cac  FUN_1010_2cac
   ============================================================ */

void __stdcall16far
FUN_1010_2cac(undefined2 param_1,undefined2 param_2,int param_3,int param_4,int param_5,
             undefined2 param_6,undefined2 param_7)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_5;
  if (param_3 == 1) {
    for (; iVar1 = param_4, iVar2 < param_5 + 3; iVar2 = iVar2 + 1) {
      for (; iVar1 < param_4 + 4; iVar1 = iVar1 + 1) {
        FUN_1008_3bd0(param_6,param_7,iVar1,iVar2,0xf0);
      }
    }
  }
  else if (param_3 == 3) {
    for (; iVar2 < param_5 + 3; iVar2 = iVar2 + 1) {
      iVar1 = iVar2 - param_5;
      FUN_1008_3bd0(param_6,param_7,param_4,iVar2,iVar1 + 0x12a);
      FUN_1008_3bd0(param_6,param_7,param_4 + 1,iVar2,iVar1 + 0x12d);
      FUN_1008_3bd0(param_6,param_7,param_4 + 2,iVar2,iVar1 + 0x12d);
      FUN_1008_3bd0(param_6,param_7,param_4 + 3,iVar2,iVar1 + 0x130);
    }
  }
  else if (param_3 == 5) {
    FUN_1010_282c(param_1,param_2);
  }
  return;
}



/* ============================================================
   1010:2e8c  FUN_1010_2e8c
   ============================================================ */

void __stdcall16far FUN_1010_2e8c(undefined4 param_1)

{
  *(undefined2 *)((int)param_1 + 0xc) = 1;
  return;
}



/* ============================================================
   1010:33b4  FUN_1010_33b4
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_33b4(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined2 uVar4;
  int iVar5;
  int iVar6;
  undefined2 uVar7;
  undefined4 local_a;
  
  FUN_1008_7560(param_1,param_2,param_3,param_4,param_5,param_6);
  *param_1 = 0x3a46;
  param_1[1] = 0x1010;
  iVar1 = *(int *)((int)*(undefined4 *)(param_1 + 2) + 0xe4);
  param_1[0x2a] = 0;
  param_1[0x2c] = 0;
  FUN_1008_9504(param_1,param_2,3,5,5);
  for (iVar6 = 0; iVar6 < (iVar1 + 2) * 2; iVar6 = iVar6 + 1) {
    uVar7 = FUN_1008_3c88(0x26);
    uVar4 = FUN_1008_3c88(0x22,uVar7);
    FUN_1008_c40a(param_1,param_2,0x20,uVar4,uVar7);
  }
  FUN_1008_ce88(param_1,param_2,0,0x1b,0x11,0xe,4);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x28);
  *(undefined2 *)((int)uVar2 + 0x28) = 0x83;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x2c);
  *(undefined2 *)((int)uVar2 + 0x28) = 0x84;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x28);
  *(undefined2 *)((int)uVar2 + 0x2a) = 0x85;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x2c);
  *(undefined2 *)((int)uVar2 + 0x2a) = 0x86;
  FUN_1008_cafa(param_1,param_2,0x60,0x1a,0x10,0xf,5);
  FUN_1008_cafa(param_1,param_2,0x60,0x17,5,0x12,3);
  FUN_1008_ce88(param_1,param_2,0,0x16,5,0x13,5);
  FUN_1008_cafa(param_1,param_2,0x60,0x17,0x12,0x12,0x10);
  FUN_1008_ce88(param_1,param_2,0,0x16,0x10,0x13,0x10);
  FUN_1008_cafa(param_1,param_2,0x60,0xf,0xd,0xd,8);
  FUN_1008_ce88(param_1,param_2,0,0xf,0xc,0xf,9);
  FUN_1008_cafa(param_1,param_2,0x60,0x1c,0xd,0x1a,8);
  FUN_1008_ce88(param_1,param_2,0,0x1a,0xc,0x1a,9);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x10);
  iVar6 = 9;
  *(undefined2 *)((int)uVar2 + 0x26) = 0x7e;
  do {
    uVar7 = 0x1b;
    iVar5 = iVar6;
    iVar3 = FUN_1008_3c88(2,0x1b,iVar6);
    if (iVar3 == 1) {
      uVar4 = 1;
    }
    else {
      uVar4 = 2;
    }
    FUN_1008_af5a(param_1[0x1b],param_1[0x1c],uVar4,uVar7,iVar5);
    uVar7 = 0xe;
    iVar5 = iVar6;
    iVar3 = FUN_1008_3c88(2,0xe,iVar6);
    if (iVar3 == 1) {
      uVar4 = 1;
    }
    else {
      uVar4 = 2;
    }
    FUN_1008_af5a(param_1[0x1b],param_1[0x1c],uVar4,uVar7,iVar5);
    iVar6 = iVar6 + 1;
  } while (iVar6 < 0xd);
  iVar6 = 0x14;
  do {
    uVar7 = 0x11;
    iVar5 = iVar6;
    iVar3 = FUN_1008_3c88(2,iVar6,0x11);
    if (iVar3 == 1) {
      uVar4 = 1;
    }
    else {
      uVar4 = 2;
    }
    FUN_1008_af5a(param_1[0x1b],param_1[0x1c],uVar4,iVar5,uVar7);
    uVar7 = 4;
    iVar5 = iVar6;
    iVar3 = FUN_1008_3c88(2,iVar6,4);
    if (iVar3 == 1) {
      uVar4 = 1;
    }
    else {
      uVar4 = 2;
    }
    FUN_1008_af5a(param_1[0x1b],param_1[0x1c],uVar4,iVar5,uVar7);
    iVar6 = iVar6 + 1;
  } while (iVar6 < 0x17);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],10,0x13,0x11);
  FUN_1008_c1e6(param_1,param_2,1,0x1f,0xe,4);
  FUN_1008_c1e6(param_1,param_2,1,0x1f,0xe,0xf);
  FUN_1008_c1e6(param_1,param_2,1,0x1f,0x19,0xf);
  FUN_1008_c1e6(param_1,param_2,1,0x1f,0x19,4);
  FUN_1008_c40a(param_1,param_2,0x21,0x13,9);
  FUN_1008_c40a(param_1,param_2,0x21,0x13,0xc);
  FUN_1008_c40a(param_1,param_2,0x21,0x16,0xc);
  FUN_1008_c40a(param_1,param_2,0x21,0x16,9);
  iVar6 = FUN_1008_3c88(3);
  local_a = (undefined2 *)CONCAT22(param_2,param_1);
  if (iVar6 < 2) {
    uVar7 = 0x21;
  }
  else {
    uVar7 = 0x60;
  }
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar2 + 0x24) = uVar7;
  iVar6 = FUN_1008_3c88(3);
  if (iVar6 < 2) {
    uVar7 = 0x21;
  }
  else {
    uVar7 = 0x60;
  }
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x34);
  *(undefined2 *)((int)uVar2 + 0x24) = uVar7;
  iVar6 = FUN_1008_3c88(3);
  if (iVar6 < 2) {
    uVar7 = 0x21;
  }
  else {
    uVar7 = 0x60;
  }
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x34);
  *(undefined2 *)((int)uVar2 + 0x2e) = uVar7;
  iVar6 = FUN_1008_3c88(3);
  if (iVar6 < 2) {
    uVar7 = 0x21;
  }
  else {
    uVar7 = 0x60;
  }
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar2 + 0x2e) = uVar7;
  FUN_1008_c40a(param_1,param_2,0x21,0x11,7);
  FUN_1008_c40a(param_1,param_2,0x21,0x11,0xe);
  FUN_1008_c40a(param_1,param_2,0x21,0x18,0xe);
  FUN_1008_c40a(param_1,param_2,0x21,0x18,7);
  iVar6 = FUN_1008_3c88(10);
  iVar5 = FUN_1008_3c88(0x22);
  uVar2 = *(undefined4 *)(iVar6 * 4 + (int)*(undefined4 *)(param_1 + 4) + 100);
  *(undefined2 *)((int)uVar2 + iVar5 * 2) = 0x23;
  param_1[0x25] = iVar1 * 3;
  param_1[0x26] = iVar1 + 2;
  return local_a;
}



/* ============================================================
   1010:38d4  FUN_1010_38d4
   ============================================================ */

void __stdcall16far FUN_1010_38d4(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar1 = *(undefined4 *)((int)param_1 + 4);
  uVar3 = (undefined2)((ulong)uVar1 >> 0x10);
  iVar2 = (int)uVar1;
  FUN_1008_67e6(*(undefined2 *)(iVar2 + 0xc),*(undefined2 *)(iVar2 + 0xe),0x3a32,0x1010);
  return;
}



/* ============================================================
   1010:3a02  FUN_1010_3a02
   ============================================================ */

void __stdcall16far FUN_1010_3a02(undefined4 param_1)

{
  *(undefined2 *)((int)param_1 + 0xc) = 1;
  return;
}



/* ============================================================
   1010:3a96  FUN_1010_3a96
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_3a96(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 unaff_SS;
  undefined2 uVar5;
  undefined1 local_28 [34];
  int local_6;
  int local_4;
  
  FUN_1008_7560(param_1,param_2,param_3,param_4,param_5,param_6);
  *param_1 = 0x3ff4;
  param_1[1] = 0x1010;
  local_6 = *(int *)((int)*(undefined4 *)(param_1 + 2) + 0xe4);
  param_1[0x2a] = 1;
  param_1[0x2b] = 1;
  param_1[0x2c] = 1;
  param_1[0x2d] = 0;
  FUN_1008_c9aa(param_1,param_2,0x23,0x27,0,0);
  local_4 = 0;
  do {
    iVar4 = 0;
    do {
      iVar2 = FUN_1008_91c6(param_1,param_2,iVar4,local_4);
      if (iVar2 != 0) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + local_4 * 4);
        *(undefined2 *)((int)uVar1 + iVar4 * 2) = 3;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x24);
    local_4 = local_4 + 1;
  } while (local_4 < 0x28);
  FUN_1010_90bc(local_28,unaff_SS,param_1 + 0x12,param_2,1,param_1,param_2,1,1);
  FUN_1010_91a2(local_28,unaff_SS);
  FUN_1008_ce88(param_1,param_2,0,0x15,0x17,0xf,0x11);
  FUN_1008_cafa(param_1,param_2,3,0x14,0x16,0x10,0x12);
  FUN_1008_ce88(param_1,param_2,0,0x1f,9,0x1b,5);
  FUN_1008_c1e6(param_1,param_2,0,0x18,0x1c,6);
  FUN_1008_ce88(param_1,param_2,0,0x1f,0x23,0x1b,0x1f);
  FUN_1008_c1e6(param_1,param_2,0,0x18,0x1c,0x20);
  FUN_1008_ce88(param_1,param_2,0,9,0x23,5,0x1f);
  FUN_1008_c1e6(param_1,param_2,0,0x18,6,0x20);
  FUN_1008_ce88(param_1,param_2,0,9,9,5,5);
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x18);
  *(undefined2 *)((int)uVar1 + 0xc) = 0x7e;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar1 + 0x10) = 0x23;
  for (iVar4 = 0; iVar2 = local_6, iVar4 < local_6; iVar4 = iVar4 + 1) {
    uVar5 = 3;
    iVar2 = FUN_1008_3c88(0x1c,3);
    iVar2 = iVar2 + 10;
    iVar3 = FUN_1008_3c88(0x18,iVar2);
    FUN_1008_80c0(param_1,param_2,0,0,0,1,iVar3 + 10,iVar2,uVar5);
  }
  iVar4 = 0;
  while (iVar3 = iVar4, iVar3 < 0x28) {
    for (iVar4 = 0; iVar4 < 0x24; iVar4 = iVar4 + 1) {
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + iVar3 * 4);
      if (*(int *)((int)uVar1 + iVar4 * 2) == 3) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + iVar3 * 4);
        *(undefined2 *)((int)uVar1 + iVar4 * 2) = 0x5d;
      }
      else {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + iVar3 * 4);
        if (*(int *)((int)uVar1 + iVar4 * 2) == 0) {
          uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + iVar3 * 4);
          *(undefined2 *)((int)uVar1 + iVar4 * 2) = 0x5e;
        }
      }
    }
    local_4 = iVar3;
    iVar4 = iVar3 + 1;
  }
  FUN_1010_3ece(param_1,param_2,9,9,5,5);
  param_1[0x25] = (iVar2 + 2) * 2;
  iVar4 = (iVar2 + -6) * -10;
  if (iVar4 < 0x14) {
    iVar4 = 0x14;
  }
  param_1[0x27] = iVar4;
  FUN_1010_9360(local_28,unaff_SS);
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:3ece  FUN_1010_3ece
   ============================================================ */

void __stdcall16far
FUN_1010_3ece(undefined4 param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_5;
  while( true ) {
    iVar2 = param_4;
    if (param_3 < iVar3) break;
    for (; iVar2 <= param_2; iVar2 = iVar2 + 1) {
      uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 * 4);
      if (*(int *)((int)uVar1 + iVar2 * 2) == 0x5e) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 * 4);
        *(undefined2 *)((int)uVar1 + iVar2 * 2) = 0;
      }
      uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 * 4);
      if (*(int *)((int)uVar1 + iVar2 * 2) == 0x5d) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar3 * 4);
        *(undefined2 *)((int)uVar1 + iVar2 * 2) = 3;
      }
      FUN_1008_a060((int)param_1,param_1._2_2_,iVar2,iVar3);
    }
    iVar3 = iVar3 + 1;
  }
  FUN_1008_c9aa((int)param_1,param_1._2_2_,param_2,param_3,param_4,param_5);
  FUN_1008_c662((int)param_1,param_1._2_2_);
  return;
}



/* ============================================================
   1010:4044  FUN_1010_4044
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_4044(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  undefined4 uVar2;
  undefined2 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  
  FUN_1008_7560(param_1,param_2,param_3,param_4,param_5,param_6);
  *param_1 = 0x4f26;
  param_1[1] = 0x1010;
  iVar1 = *(int *)((int)*(undefined4 *)(param_1 + 2) + 0xe4);
  FUN_1008_b164(param_1,param_2,0,0x1048);
  FUN_1010_4a0e(param_1,param_2,1);
  FUN_1010_4a0e(param_1,param_2,1);
  FUN_1010_4a0e(param_1,param_2,1);
  uVar9 = 3;
  uVar8 = 0x1c;
  uVar7 = 1;
  uVar3 = FUN_1008_3c88(iVar1,1,0x1c,3);
  FUN_1008_80c0(param_1,param_2,0,0,0,uVar3,uVar7,uVar8,uVar9);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],9,0,0x24);
  iVar4 = FUN_1008_3c88(3);
  if (1 < iVar4) {
    FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0,0x23);
  }
  iVar4 = FUN_1008_3c88(2);
  if (iVar4 == 2) {
    FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0,0x22);
  }
  iVar4 = FUN_1008_3c88(3);
  if (1 < iVar4) {
    FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,8,0);
  }
  iVar4 = FUN_1008_3c88(3);
  if (iVar4 < 3) {
    FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,9,0);
  }
  FUN_1008_bf2a(param_1,param_2,5,0x1f);
  FUN_1008_bf2a(param_1,param_2,9,0x1f);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x90);
  *(undefined2 *)((int)uVar2 + 6) = 0x69;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x90);
  *(undefined2 *)((int)uVar2 + 8) = 0x6b;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x8c);
  *(undefined2 *)((int)uVar2 + 8) = 0x6c;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x90);
  *(undefined2 *)((int)uVar2 + 0x18) = 0x6a;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x8c);
  *(undefined2 *)((int)uVar2 + 0x18) = 0x6c;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x90);
  iVar4 = 0;
  *(undefined2 *)((int)uVar2 + 0x1a) = 0x69;
  do {
    iVar5 = iVar4 + 99;
    iVar6 = iVar4 * 6;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x98);
    iVar4 = iVar4 + 1;
    *(int *)((int)uVar2 + iVar6 + 2) = iVar5;
  } while (iVar4 < 6);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x8c);
  *(undefined2 *)((int)uVar2 + 0x10) = 0x29;
  *(undefined2 *)((int)**(undefined4 **)(param_1 + 4) + 0x46) = 0x88;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 4);
  *(undefined2 *)((int)uVar2 + 0x46) = 0x89;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 8);
  *(undefined2 *)((int)uVar2 + 0x36) = 0x2a;
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x2d,0x13,0x26);
  FUN_1008_c1e6(param_1,param_2,1,0x1e,0x14,0x1f);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0xc);
  *(undefined2 *)((int)uVar2 + 0x22) = 0x27;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x80);
  iVar4 = 0x25;
  *(undefined2 *)((int)uVar2 + 0x3e) = 0x28;
  do {
    iVar5 = FUN_1008_3c88(3);
    FUN_1008_80c0(param_1,param_2,0,1,3,1,0x18,iVar4,iVar5 == 1);
    iVar4 = iVar4 + -1;
  } while (0x1b < iVar4);
  iVar4 = 0x21;
  do {
    iVar5 = FUN_1008_3c88(3);
    FUN_1008_80c0(param_1,param_2,1,0,3,1,iVar4,0x27,iVar5 == 1);
    iVar4 = iVar4 + -1;
  } while (0x17 < iVar4);
  iVar4 = 0x1e;
  do {
    iVar5 = FUN_1008_3c88(3);
    FUN_1008_80c0(param_1,param_2,0,0xffff,3,1,0x23,iVar4,iVar5 == 1);
    iVar4 = iVar4 + 1;
  } while (iVar4 < 0x28);
  iVar4 = 0x1a;
  do {
    iVar5 = FUN_1008_3c88(3);
    FUN_1008_80c0(param_1,param_2,0xffff,0,3,1,iVar4,0x1c,iVar5 == 1);
    iVar4 = iVar4 + 1;
  } while (iVar4 < 0x24);
  param_1[0x25] = iVar1 + 0x18;
  param_1[0x26] = iVar1;
  param_1[0x27] = 0x19;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:4382  FUN_1010_4382
   ============================================================ */

void __stdcall16far FUN_1010_4382(int param_1,undefined2 param_2,int param_3)

{
  undefined2 uVar1;
  int in_DX;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 unaff_SS;
  undefined4 uVar6;
  undefined2 *puVar7;
  undefined1 local_14 [4];
  undefined2 local_10;
  undefined2 local_e;
  int local_c;
  undefined2 local_8;
  int local_6;
  int local_4;
  
  local_c = param_3 + -0x27;
  iVar3 = in_DX;
  if (*(int *)(param_1 + 0x16) == 0 && *(int *)(param_1 + 0x14) == 0) {
    local_6 = FUN_1000_1646(0x54);
    local_4 = in_DX;
    if (in_DX == 0 && local_6 == 0) {
      *(undefined2 *)(param_1 + 0x16) = 0;
      *(undefined2 *)(param_1 + 0x14) = 0;
      iVar3 = 0;
    }
    else {
      uVar6 = FUN_1010_5fc4(local_6,in_DX,0,*(int *)(param_1 + 0x3c) + 1,
                            *(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6));
      iVar3 = (int)((ulong)uVar6 >> 0x10);
      *(undefined2 *)(param_1 + 0x14) = (int)uVar6;
      *(int *)(param_1 + 0x16) = iVar3;
    }
    uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x14) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 0x14);
    *(int *)(iVar2 + 0xe) = param_1;
    *(undefined2 *)(iVar2 + 0x10) = param_2;
  }
  if (*(int *)(param_1 + 0x1a) == 0 && *(int *)(param_1 + 0x18) == 0) {
    local_6 = FUN_1000_1646(100);
    local_4 = iVar3;
    if (iVar3 == 0 && local_6 == 0) {
      *(undefined2 *)(param_1 + 0x1a) = 0;
      *(undefined2 *)(param_1 + 0x18) = 0;
      iVar3 = 0;
    }
    else {
      uVar6 = FUN_1010_6a56(local_6,iVar3,1,*(int *)(param_1 + 0x3c) + 1,
                            *(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6));
      iVar3 = (int)((ulong)uVar6 >> 0x10);
      *(undefined2 *)(param_1 + 0x18) = (int)uVar6;
      *(int *)(param_1 + 0x1a) = iVar3;
    }
    uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x18) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 0x18);
    *(int *)(iVar2 + 0xe) = param_1;
    *(undefined2 *)(iVar2 + 0x10) = param_2;
  }
  if (*(int *)(param_1 + 0x1e) == 0 && *(int *)(param_1 + 0x1c) == 0) {
    local_6 = FUN_1000_1646(0xe8);
    local_4 = iVar3;
    if (iVar3 == 0 && local_6 == 0) {
      *(undefined2 *)(param_1 + 0x1e) = 0;
      *(undefined2 *)(param_1 + 0x1c) = 0;
      iVar3 = 0;
    }
    else {
      uVar6 = FUN_1010_0ea0(local_6,iVar3,2,*(int *)(param_1 + 0x3c) + 1,
                            *(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6));
      iVar3 = (int)((ulong)uVar6 >> 0x10);
      *(undefined2 *)(param_1 + 0x1c) = (int)uVar6;
      *(int *)(param_1 + 0x1e) = iVar3;
    }
    uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x1c) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 0x1c);
    *(int *)(iVar2 + 0xe) = param_1;
    *(undefined2 *)(iVar2 + 0x10) = param_2;
  }
  if (*(int *)(param_1 + 0x22) == 0 && *(int *)(param_1 + 0x20) == 0) {
    local_6 = FUN_1000_1646(0x56);
    local_4 = iVar3;
    if (iVar3 == 0 && local_6 == 0) {
      *(undefined2 *)(param_1 + 0x22) = 0;
      *(undefined2 *)(param_1 + 0x20) = 0;
      iVar3 = 0;
    }
    else {
      uVar6 = FUN_1010_cb9c(local_6,iVar3,1,3,*(int *)(param_1 + 0x3c) + 1,
                            *(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6));
      iVar3 = (int)((ulong)uVar6 >> 0x10);
      *(undefined2 *)(param_1 + 0x20) = (int)uVar6;
      *(int *)(param_1 + 0x22) = iVar3;
    }
    uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x20) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 0x20);
    *(int *)(iVar2 + 0xe) = param_1;
    *(undefined2 *)(iVar2 + 0x10) = param_2;
  }
  uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x14) >> 0x10);
  iVar2 = (int)*(undefined4 *)(param_1 + 0x14);
  if (*(int *)(iVar2 + 0x16) == 0 && *(int *)(iVar2 + 0x14) == 0) {
    local_6 = FUN_1000_1646(0x66);
    local_4 = iVar3;
    if (iVar3 == 0 && local_6 == 0) {
      uVar6 = 0;
    }
    else {
      uVar6 = FUN_1010_55ac(local_6,iVar3,*(int *)(param_1 + 0x3c) + 2,*(undefined2 *)(param_1 + 4),
                            *(undefined2 *)(param_1 + 6));
    }
    local_8 = (undefined2)((ulong)uVar6 >> 0x10);
    iVar3 = (int)uVar6;
    uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x14) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 0x14);
    *(int *)(iVar2 + 0x14) = iVar3;
    *(undefined2 *)(iVar2 + 0x16) = local_8;
    uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x18) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 0x18);
    *(int *)(iVar2 + 0x14) = iVar3;
    *(undefined2 *)(iVar2 + 0x16) = local_8;
    uVar4 = *(undefined2 *)(param_1 + 0x16);
    *(undefined2 *)(iVar3 + 0xe) = *(undefined2 *)(param_1 + 0x14);
    *(undefined2 *)(iVar3 + 0x10) = uVar4;
    uVar4 = *(undefined2 *)(param_1 + 0x1a);
    *(undefined2 *)(iVar3 + 0x62) = *(undefined2 *)(param_1 + 0x18);
    *(undefined2 *)(iVar3 + 100) = uVar4;
  }
  if (param_3 < 0x2b) {
    uVar4 = *(undefined2 *)(param_1 + 0x14 + local_c * 4);
    uVar1 = *(undefined2 *)(param_1 + 0x14 + local_c * 4 + 2);
    local_4 = uVar1;
    puVar7 = (undefined2 *)FUN_1008_b946(uVar4,uVar1,0,local_14,unaff_SS);
    local_10 = *puVar7;
    local_e = ((undefined2 *)puVar7)[1];
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 4) + 0xea) = local_10;
    *(undefined2 *)((int)*(undefined4 *)(param_1 + 4) + 0xec) = local_e;
    uVar5 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
    iVar3 = (int)*(undefined4 *)(param_1 + 4);
    *(undefined2 *)(iVar3 + 0xdc) = uVar4;
    *(undefined2 *)(iVar3 + 0xde) = uVar1;
    FUN_1008_a5fa();
    DAT_1080_1070 = 1;
    FUN_1008_a652(uVar4,uVar1);
    REDRAWWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,0);
  }
  else {
    FUN_1008_2d02(*(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6),0,0x4e3e,0x1010);
  }
  return;
}



/* ============================================================
   1010:4692  FUN_1010_4692
   ============================================================ */

void __stdcall16far
FUN_1010_4692(undefined4 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined2 uVar4;
  long lVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  
  iVar1 = *(int *)((int)*(undefined4 *)((int)param_1 + 4) + 0xe4);
  iVar3 = (6 - iVar1) * 10;
  lVar5 = FUN_1000_2024(*(undefined2 *)((int)param_1 + 0x3e),*(undefined2 *)((int)param_1 + 0x40),
                        iVar3,iVar3 >> 0xf);
  if (lVar5 == 0) {
    FUN_1010_4a0e((int)param_1,param_1._2_2_,0);
  }
  lVar5 = FUN_1000_2024(*(undefined2 *)((int)param_1 + 0x3e),*(undefined2 *)((int)param_1 + 0x40),
                        200,0);
  if (lVar5 == 0xb4) {
    *(undefined2 *)((int)param_1 + 0x4e) = 1;
  }
  lVar5 = FUN_1000_2024(*(undefined2 *)((int)param_1 + 0x3e),*(undefined2 *)((int)param_1 + 0x40),
                        200,0);
  if (lVar5 == 0) {
    *(undefined2 *)((int)param_1 + 0x4e) = 0x19;
  }
  FUN_1008_cdbe((int)param_1,param_1._2_2_,0,1,0x18,0x1c,0x18,0x1c);
  FUN_1008_cdbe((int)param_1,param_1._2_2_,1,0,0x18,0x27,0x18,0x27);
  FUN_1008_cdbe((int)param_1,param_1._2_2_,0,0xffff,0x23,0x27,0x23,0x27);
  FUN_1008_cdbe((int)param_1,param_1._2_2_,0xffff,0,0x23,0x1c,0x23,0x1c);
  if ((*(int *)((int)param_1 + 0x3e) == 6) && (*(int *)((int)param_1 + 0x40) == 0)) {
    *(int *)((int)param_1 + 0x4a) = iVar1 + 4;
  }
  if (((*(int *)((int)*(undefined4 *)((int)param_1 + 4) + 0xea) == 0x26) &&
      (*(int *)((int)*(undefined4 *)((int)param_1 + 4) + 0xec) == 0x13)) &&
     (uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x78),
     *(int *)((int)uVar2 + 0x38) == 0x38)) {
    FUN_1008_ce88((int)param_1,param_1._2_2_,0,0x1f,0x1f,0x1c,0x1e);
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x7c);
    *(undefined2 *)((int)uVar2 + 0x38) = 0x3b;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x78);
    *(undefined2 *)((int)uVar2 + 0x38) = 0x36;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x74);
    *(undefined2 *)((int)uVar2 + 0x38) = 0x3a;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x74);
    *(undefined2 *)((int)uVar2 + 0x36) = 0x37;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x7c);
    *(undefined2 *)((int)uVar2 + 0x3e) = 0x39;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x78);
    *(undefined2 *)((int)uVar2 + 0x3e) = 0x36;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x74);
    *(undefined2 *)((int)uVar2 + 0x3e) = 0x38;
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x74);
    *(undefined2 *)((int)uVar2 + 0x40) = 0x37;
    for (iVar3 = 0x1d; iVar3 < 0x20; iVar3 = iVar3 + 1) {
      FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),0x16,
                    0x1c,iVar3);
      FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),0x16,
                    0x1f,iVar3);
    }
    FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),0x16,
                  0x20,0x1d);
    FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),0x16,
                  0x1b,0x1d);
    FUN_1008_bb7e((int)param_1,param_1._2_2_,1,0,0,0x1e,0x1f,0x1d,0x1e);
    FUN_1008_bb7e((int)param_1,param_1._2_2_,1,0,0x5f,0x17,0x22,0x13,0x1e);
    uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x80);
    *(undefined2 *)((int)uVar2 + 0x2a) = 0x18;
    uVar8 = 0;
    uVar7 = 0x20;
    uVar6 = 0x15;
    uVar4 = FUN_1008_3c88(10,0x15,0x20,0);
    FUN_1008_80c0((int)param_1,param_1._2_2_,0,0,0,uVar4,uVar6,uVar7,uVar8);
    uVar8 = 1;
    uVar7 = 0x20;
    uVar6 = 0x15;
    uVar4 = FUN_1008_3c88(4,0x15,0x20,1);
    FUN_1008_80c0((int)param_1,param_1._2_2_,0,0,0,uVar4,uVar6,uVar7,uVar8);
    uVar8 = 2;
    uVar7 = 0x20;
    uVar6 = 0x15;
    uVar4 = FUN_1008_3c88(iVar1 * 2,0x15,0x20,2);
    FUN_1008_80c0((int)param_1,param_1._2_2_,0,0,0,uVar4,uVar6,uVar7,uVar8);
    FUN_1008_31d4(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0x19);
  }
  FUN_1008_ba30((int)param_1,param_1._2_2_,param_2,param_3,param_4);
  return;
}



/* ============================================================
   1010:4a0e  FUN_1010_4a0e
   ============================================================ */

void __stdcall16far FUN_1010_4a0e(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined2 uVar11;
  int local_e;
  int local_c;
  int local_a;
  int local_4;
  
  uVar11 = (undefined2)((ulong)param_1 >> 0x10);
  iVar7 = (int)param_1;
  iVar6 = *(int *)((int)*(undefined4 *)(iVar7 + 4) + 0xe4);
  local_e = 0;
  local_a = 0xc;
  do {
    for (iVar8 = 7; iVar8 < 0x17; iVar8 = iVar8 + 1) {
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + local_a * 4);
      if (*(int *)((int)uVar1 + iVar8 * 2) == 0x18) {
        local_e = local_e + 1;
      }
    }
    local_a = local_a + 1;
  } while (local_a < 0x1d);
  if (local_e < 3) {
    local_c = 0x14;
    do {
      bVar2 = true;
      iVar3 = FUN_1008_3c88(0xf);
      iVar4 = FUN_1008_3c88(0xe);
      iVar5 = iVar4 + 6;
      iVar10 = iVar3 + 0xb;
      for (iVar8 = iVar10; iVar9 = iVar5, iVar8 < iVar3 + 0xe; iVar8 = iVar8 + 1) {
        for (; iVar9 < iVar4 + 9; iVar9 = iVar9 + 1) {
          uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + iVar8 * 4);
          if ((((0x7e < *(int *)((int)uVar1 + iVar9 * 2)) &&
               (uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + iVar8 * 4),
               *(int *)((int)uVar1 + iVar9 * 2) < 0x83)) ||
              ((uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + iVar8 * 4),
               7 < *(int *)((int)uVar1 + iVar9 * 2) &&
               (uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + iVar8 * 4),
               *(int *)((int)uVar1 + iVar9 * 2) < 0x18)))) ||
             ((*(int *)((int)*(undefined4 *)(iVar7 + 4) + 0xea) == iVar8 &&
              (*(int *)((int)*(undefined4 *)(iVar7 + 4) + 0xec) == iVar9)))) {
            bVar2 = false;
            break;
          }
        }
        if (!bVar2) break;
      }
      local_c = local_c + -1;
    } while ((!bVar2) || (local_c < 1));
    if ((0 < local_c) && (FUN_1008_c1e6(iVar7,uVar11,1,0x18,iVar5,iVar10), param_2 == 0)) {
      for (; iVar8 = iVar5, iVar10 < iVar3 + 0xe; iVar10 = iVar10 + 1) {
        for (; iVar8 < iVar4 + 9; iVar8 = iVar8 + 1) {
          FUN_1008_af5a(*(undefined2 *)(iVar7 + 0x36),*(undefined2 *)(iVar7 + 0x38),0x16,iVar8,
                        iVar10);
        }
      }
      FUN_1008_31d4(*(undefined2 *)(iVar7 + 4),*(undefined2 *)(iVar7 + 6),0x16);
      if ((*(int *)(iVar7 + 0x4a) < 0x4b) && (iVar8 = FUN_1008_3c88(iVar6 + 2), iVar8 < iVar6 + 1))
      {
        *(int *)(iVar7 + 0x4a) = *(int *)(iVar7 + 0x4a) + 1;
      }
      if (local_e == 0) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + 8);
        if ((*(int *)((int)uVar1 + 0x12) == 0x60) && (iVar6 = FUN_1008_3c88(5), iVar6 == 5)) {
          uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + 8);
          *(undefined2 *)((int)uVar1 + 0x12) = 0x5c;
          FUN_1008_af5a(*(undefined2 *)(iVar7 + 0x36),*(undefined2 *)(iVar7 + 0x38),0x16,9,2);
        }
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + 0x7c);
        if ((*(int *)((int)uVar1 + 8) == 0x5f) && (iVar6 = FUN_1008_3c88(0xc), iVar6 == 3)) {
          uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + 0x7c);
          *(undefined2 *)((int)uVar1 + 8) = 0x5c;
          FUN_1008_af5a(*(undefined2 *)(iVar7 + 0x36),*(undefined2 *)(iVar7 + 0x38),0x16,4,0x1f);
        }
        iVar6 = FUN_1008_3c88(10);
        if (iVar6 == 2) {
          local_c = 0x14;
          do {
            bVar2 = true;
            iVar8 = FUN_1008_3c88(0xf);
            iVar3 = FUN_1008_3c88(0xe);
            iVar4 = iVar3 + 6;
            local_4 = iVar8 + 0xb;
            for (iVar6 = local_4; iVar5 = iVar4, iVar6 < iVar8 + 0xd; iVar6 = iVar6 + 1) {
              for (; iVar5 < iVar3 + 8; iVar5 = iVar5 + 1) {
                uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + iVar6 * 4);
                if ((((0x7e < *(int *)((int)uVar1 + iVar5 * 2)) &&
                     (uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + iVar6 * 4),
                     *(int *)((int)uVar1 + iVar5 * 2) < 0x83)) ||
                    ((uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + iVar6 * 4),
                     7 < *(int *)((int)uVar1 + iVar5 * 2) &&
                     (uVar1 = *(undefined4 *)((int)*(undefined4 *)(iVar7 + 8) + iVar6 * 4),
                     *(int *)((int)uVar1 + iVar5 * 2) < 0x18)))) ||
                   ((*(int *)((int)*(undefined4 *)(iVar7 + 4) + 0xea) == iVar6 &&
                    (*(int *)((int)*(undefined4 *)(iVar7 + 4) + 0xec) == iVar5)))) {
                  bVar2 = false;
                  break;
                }
              }
              if (!bVar2) break;
            }
            local_c = local_c + -1;
          } while ((!bVar2) && (0 < local_c));
          if (0 < local_c) {
            FUN_1008_ce88(iVar7,uVar11,0,iVar3 + 7,iVar8 + 0xc,iVar4,local_4);
            FUN_1008_c452(iVar7,uVar11,iVar4,local_4);
            for (; iVar6 = iVar4, local_4 < iVar8 + 0xd; local_4 = local_4 + 1) {
              for (; iVar6 < iVar3 + 8; iVar6 = iVar6 + 1) {
                FUN_1008_af5a(*(undefined2 *)(iVar7 + 0x36),*(undefined2 *)(iVar7 + 0x38),0x16,iVar6
                              ,local_4);
              }
            }
          }
        }
      }
    }
  }
  return;
}



/* ============================================================
   1010:4f76  FUN_1010_4f76
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_4f76(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  int iVar6;
  
  FUN_1008_7560((undefined2 *)param_1,param_1._2_2_,param_2,param_3,param_4,param_5);
  *param_1 = 0x555c;
  ((undefined2 *)param_1)[1] = 0x1010;
  iVar1 = *(int *)((int)*(undefined4 *)((undefined2 *)param_1 + 2) + 0xe4);
  FUN_1008_b164((undefined2 *)param_1,param_1._2_2_,0,0x1050);
  FUN_1008_bb5a((undefined2 *)param_1,param_1._2_2_,0,0x21,0x20);
  ((undefined2 *)param_1)[0x2a] = 0x18;
  ((undefined2 *)param_1)[0x2b] = 0;
  FUN_1008_c452((undefined2 *)param_1,param_1._2_2_,5,0x21);
  FUN_1008_c452((undefined2 *)param_1,param_1._2_2_,0x1d,5);
  for (iVar6 = 0; iVar6 < iVar1 + 5; iVar6 = iVar6 + 1) {
    iVar2 = FUN_1008_3c88(0x18);
    iVar2 = iVar2 + 10;
    iVar3 = FUN_1008_3c88(7);
    iVar3 = iVar3 + iVar2 + -4;
    iVar4 = FUN_1008_3c88(7,iVar3,iVar2);
    if (iVar4 < 2) {
      uVar5 = 2;
    }
    else {
      uVar5 = 1;
    }
    FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],uVar5,iVar3,iVar2);
  }
  ((undefined2 *)param_1)[0x25] = (iVar1 + 3) * 2;
  return param_1;
}



/* ============================================================
   1010:5062  FUN_1010_5062
   ============================================================ */

void __stdcall16far FUN_1010_5062(undefined4 param_1,int param_2)

{
  undefined2 uVar1;
  int iVar2;
  int in_DX;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 unaff_SS;
  undefined4 uVar5;
  undefined2 *puVar6;
  undefined1 local_e [4];
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_4;
  
  if (param_2 < 0x28) {
    if (*(int *)((int)param_1 + 0x16) == 0 && *(int *)((int)param_1 + 0x14) == 0) {
      iVar2 = FUN_1000_1646(0x5a);
      if (in_DX == 0 && iVar2 == 0) {
        *(undefined2 *)((int)param_1 + 0x16) = 0;
        *(undefined2 *)((int)param_1 + 0x14) = 0;
      }
      else {
        uVar5 = FUN_1010_33b4(iVar2,in_DX,0,*(int *)((int)param_1 + 0x3c) + 1,
                              *(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6));
        *(undefined2 *)((int)param_1 + 0x14) = (int)uVar5;
        *(undefined2 *)((int)param_1 + 0x16) = (int)((ulong)uVar5 >> 0x10);
      }
      uVar3 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x14) >> 0x10);
      iVar2 = (int)*(undefined4 *)((int)param_1 + 0x14);
      *(int *)(iVar2 + 0xe) = (int)param_1;
      *(undefined2 *)(iVar2 + 0x10) = param_1._2_2_;
    }
    uVar3 = *(undefined2 *)((int)param_1 + 0x14);
    uVar1 = *(undefined2 *)((int)param_1 + 0x16);
    local_4 = uVar1;
    puVar6 = (undefined2 *)FUN_1008_b946(uVar3,uVar1,0,local_e,unaff_SS);
    local_a = *puVar6;
    local_8 = ((undefined2 *)puVar6)[1];
    *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xea) = local_a;
    *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xec) = local_8;
    uVar4 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 4) >> 0x10);
    iVar2 = (int)*(undefined4 *)((int)param_1 + 4);
    *(undefined2 *)(iVar2 + 0xdc) = uVar3;
    *(undefined2 *)(iVar2 + 0xde) = uVar1;
    FUN_1008_a5fa();
    DAT_1080_1070 = 1;
    FUN_1008_a652(uVar3,uVar1);
    REDRAWWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,0);
  }
  else {
    FUN_1008_2d02(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0,0xdfae,
                  (char *)s_Lucky_Chip_1080_1004 + 4);
  }
  return;
}



/* ============================================================
   1010:518c  FUN_1010_518c
   ============================================================ */

void __stdcall16far
FUN_1010_518c(undefined4 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined2 uVar8;
  
  iVar7 = *(int *)((int)*(undefined4 *)((int)param_1 + 4) + 0xe4);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x84);
  if ((*(int *)((int)uVar2 + 10) != 0x7f) && (*(int *)((int)param_1 + 0x42) == 0)) {
    iVar4 = 0;
    do {
      iVar5 = FUN_1008_3c88(0x18);
      iVar5 = iVar5 + 10;
      iVar6 = FUN_1008_3c88(7);
      iVar6 = iVar6 + iVar5 + -4;
      iVar3 = FUN_1008_ce4e((int)param_1,param_1._2_2_,iVar6,iVar5,iVar6,iVar5);
      if (iVar3 == 0) {
        uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar5 * 4);
        *(undefined2 *)((int)uVar2 + iVar6 * 2) = 0x22;
        FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),0x16
                      ,iVar6,iVar5);
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 7);
    *(undefined2 *)((int)param_1 + 0x42) = 1;
  }
  uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + 0x14);
  if ((*(int *)((int)uVar2 + 0x3a) != 0x7f) && (*(int *)((int)param_1 + 0x44) == 0)) {
    iVar4 = 0;
    do {
      iVar5 = FUN_1008_3c88(0x18);
      iVar5 = iVar5 + 10;
      iVar6 = FUN_1008_3c88(7);
      iVar6 = iVar6 + iVar5 + -4;
      iVar3 = FUN_1008_ce4e((int)param_1,param_1._2_2_,iVar6,iVar5,iVar6,iVar5);
      if (iVar3 == 0) {
        uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar5 * 4);
        *(undefined2 *)((int)uVar2 + iVar6 * 2) = 0x22;
        FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),0x16
                      ,iVar6,iVar5);
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 7);
    *(undefined2 *)((int)param_1 + 0x44) = 1;
  }
  if ((-1 < *(int *)((int)param_1 + 0x56)) &&
     ((0 < *(int *)((int)param_1 + 0x56) || (*(int *)((int)param_1 + 0x54) != 0)))) {
    iVar4 = FUN_1008_3c88(iVar7 * 2);
    if (1 < iVar4) {
      FUN_1008_80c0((int)param_1,param_1._2_2_,0xffff,0xffff,3,1,0x20,0x1c,4);
    }
    iVar4 = FUN_1008_3c88(iVar7 * 2);
    if (1 < iVar4) {
      FUN_1008_80c0((int)param_1,param_1._2_2_,0xffff,0xffff,3,1,0x21,0x1f,4);
    }
    iVar4 = FUN_1008_3c88(iVar7 * 2);
    if (1 < iVar4) {
      FUN_1008_80c0((int)param_1,param_1._2_2_,0xffff,0xffff,3,1,0x1e,0x25,4);
    }
    iVar7 = FUN_1008_3c88(iVar7 * 2);
    if (1 < iVar7) {
      FUN_1008_80c0((int)param_1,param_1._2_2_,0xffff,0xffff,3,1,0x1b,0x23,4);
    }
    piVar1 = (int *)((int)param_1 + 0x54);
    iVar7 = *piVar1;
    *piVar1 = *piVar1 + -1;
    *(int *)((int)param_1 + 0x56) = *(int *)((int)param_1 + 0x56) - (uint)(iVar7 == 0);
    if (*(int *)((int)param_1 + 0x56) == 0 && *(int *)((int)param_1 + 0x54) == 0) {
      FUN_1008_ccfe((int)param_1,param_1._2_2_,0x23,0x27,0,0);
      FUN_1008_bb5a((int)param_1,param_1._2_2_,0,0x20,0x21);
    }
  }
  if ((-1 < *(int *)((int)param_1 + 0x40)) &&
     (((0 < *(int *)((int)param_1 + 0x40) || (800 < *(uint *)((int)param_1 + 0x3e))) &&
      (*(int *)((int)param_1 + 0x46) == 0)))) {
    iVar7 = 0;
    do {
      iVar4 = FUN_1008_3c88(0x18);
      iVar4 = iVar4 + 10;
      iVar5 = FUN_1008_3c88(7);
      iVar5 = iVar5 + iVar4 + -4;
      iVar6 = FUN_1008_ce4e((int)param_1,param_1._2_2_,iVar5,iVar4,iVar5,iVar4);
      if (iVar6 == 0) {
        uVar2 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4);
        *(undefined2 *)((int)uVar2 + iVar5 * 2) = 0x22;
        FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),0x16
                      ,iVar5,iVar4);
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < 0x18);
    if (0 < *(int *)((int)param_1 + 0x50)) {
      iVar7 = *(int *)((int)param_1 + 0x50);
      while (iVar7 = iVar7 + -1, 0 < iVar7) {
        uVar8 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x2c) >> 0x10);
        iVar4 = (int)*(undefined4 *)((int)param_1 + 0x2c) + iVar7 * 0xc;
        FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),0x16
                      ,*(undefined2 *)(iVar4 + 4),*(undefined2 *)(iVar4 + 2));
        FUN_1008_8032((int)param_1,param_1._2_2_,iVar7);
      }
    }
    *(undefined2 *)((int)param_1 + 0x50) = 0;
    *(undefined2 *)((int)param_1 + 0x46) = 1;
  }
  FUN_1008_ba30((int)param_1,param_1._2_2_,param_2,param_3,param_4);
  return;
}



/* ============================================================
   1010:55ac  FUN_1010_55ac
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_55ac(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  undefined4 uVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined2 unaff_SS;
  int local_14 [7];
  int local_6;
  int local_4;
  
  FUN_1008_7560(param_1,param_2,0,param_3,param_4,param_5);
  *param_1 = 0x5f74;
  param_1[1] = 0x1010;
  local_14[6] = *(int *)((int)*(undefined4 *)(param_1 + 2) + 0xe4);
  FUN_1008_b164(param_1,param_2,0,0x1058);
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0xc);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x23;
  local_14[2] = 0x15;
  local_14[0] = 0x12;
  local_14[1] = 0x12;
  local_14[3] = 0x12;
  local_14[4] = 0x13;
  local_14[5] = 0xf;
  iVar5 = 0;
  do {
    iVar3 = local_14[iVar5 * 2];
    FUN_1008_c1e6(param_1,param_2,1,0x19,local_14[iVar5 * 2 + 1],iVar3);
    local_6 = iVar5;
    for (local_4 = iVar3; local_4 < local_14[iVar5 * 2] + 3; local_4 = local_4 + 2) {
      for (iVar3 = local_14[iVar5 * 2 + 1]; iVar3 < local_14[iVar5 * 2 + 1] + 3; iVar3 = iVar3 + 2)
      {
        if ((local_14[iVar5 * 2] - local_4 != -1) || (local_14[iVar5 * 2 + 1] - iVar3 != -1)) {
          FUN_1008_80c0(param_1,param_2,0,0,0,1,iVar3,local_4,1);
        }
      }
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 3);
  do {
    do {
      iVar5 = FUN_1008_3c88(0x27);
      param_1[0x2c] = iVar5 + -1;
      iVar5 = FUN_1008_3c88(0x24);
      param_1[0x2d] = iVar5 + -1;
      uVar1 = *(undefined4 *)(param_1[0x2c] * 4 + (int)*(undefined4 *)(param_1 + 4));
    } while (*(int *)((int)uVar1 + (iVar5 + -1) * 2) != 3);
    uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + param_1[0x2c] * 4 + 4);
  } while (*(int *)((int)uVar1 + param_1[0x2d] * 2) != 3);
  param_1[0x2e] = 0;
  param_1[0x2f] = 0;
  param_1[0x30] = 0;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x8c);
  *(undefined2 *)((int)uVar1 + 6) = 0x24;
  param_1[0x2a] = 0;
  param_1[0x2b] = 0;
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x2d,2,0x25);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x2d,3,0x25);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0xf,5,0x27);
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x80);
  *(undefined2 *)((int)uVar1 + 0xe) = 0x27;
  for (iVar5 = 0; iVar5 < local_14[6] + 5; iVar5 = iVar5 + 1) {
    uVar2 = FUN_1008_3c88(0x1e);
    iVar3 = FUN_1008_3c88(5,uVar2);
    iVar3 = -(iVar3 + -0x24);
    iVar4 = FUN_1008_3c88(5,iVar3);
    if (iVar4 != 2) {
      iVar4 = 1;
    }
    FUN_1008_af5a(param_1[0x1b],param_1[0x1c],iVar4,iVar3,uVar2);
  }
  param_1[0x25] = local_14[6] + 10;
  param_1[0x27] = -(local_14[6] * 10 + -0x55);
  param_1[0x26] = 1;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:596a  FUN_1010_596a
   ============================================================ */

void __stdcall16far FUN_1010_596a(undefined4 param_1,int param_2)

{
  undefined2 uVar1;
  int iVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 unaff_SS;
  undefined2 *puVar5;
  undefined1 local_10 [4];
  undefined2 local_c;
  undefined2 local_a;
  undefined2 local_6;
  int local_4;
  
  local_4 = param_2 + -0x23;
  uVar3 = (undefined2)((ulong)param_1 >> 0x10);
  if (param_2 < 0x25) {
    if (local_4 == 0) {
      uVar1 = *(undefined2 *)((int)param_1 + 0xe);
      local_6 = *(undefined2 *)((int)param_1 + 0x10);
    }
    else {
      uVar1 = *(undefined2 *)((int)param_1 + 0x62);
      local_6 = *(undefined2 *)((int)param_1 + 100);
    }
    puVar5 = (undefined2 *)FUN_1008_b9ba(uVar1,local_6,0,local_10,unaff_SS);
    local_c = *puVar5;
    local_a = ((undefined2 *)puVar5)[1];
    *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xea) = local_c;
    *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xec) = local_a;
    uVar4 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 4) >> 0x10);
    iVar2 = (int)*(undefined4 *)((int)param_1 + 4);
    *(undefined2 *)(iVar2 + 0xdc) = uVar1;
    *(undefined2 *)(iVar2 + 0xde) = local_6;
    FUN_1008_a5fa();
    DAT_1080_1070 = 1;
    FUN_1008_a652(uVar1,local_6);
    REDRAWWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,0);
  }
  else {
    FUN_1008_2d02(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0,0x5f3a,
                  0x1010);
  }
  return;
}



/* ============================================================
   1010:5fc4  FUN_1010_5fc4
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_5fc4(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  FUN_1008_7560(param_1,param_2,param_3,param_4,param_5,param_6);
  *param_1 = 0x6a06;
  param_1[1] = 0x1010;
  iVar1 = *(int *)((int)*(undefined4 *)(param_1 + 2) + 0xe4);
  FUN_1008_b164(param_1,param_2,0,0x1060);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0xd,10,0x1f);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,10,0x1f);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],9,10,0x1f);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],10,0x17,0x1f);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0x17,0x1f);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],9,0x17,0x1f);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x2d,9,0x19);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],0x2d,0x18,0x19);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x28);
  *(undefined2 *)((int)uVar2 + 0x22) = 0x59;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x28);
  *(undefined2 *)((int)uVar2 + 0x20) = 0x62;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x28);
  *(undefined2 *)((int)uVar2 + 0x24) = 0x62;
  FUN_1008_c1e6(param_1,param_2,0,0x1f,0xe,0x11);
  FUN_1008_c1e6(param_1,param_2,0,0x1f,0x13,0x11);
  iVar4 = 10;
  do {
    iVar3 = iVar1 * 3;
    FUN_1008_80c0(param_1,param_2,0,0,2,iVar3,iVar4,5,4);
    FUN_1008_80c0(param_1,param_2,0,0,2,iVar3,iVar4,0x22,4);
    FUN_1008_80c0(param_1,param_2,0,0,2,iVar3,5,iVar4,4);
    FUN_1008_80c0(param_1,param_2,0,0,2,iVar3,0x1f,iVar4,4);
    iVar4 = iVar4 + 5;
  } while (iVar4 < 0x1f);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x14);
  *(undefined2 *)((int)uVar2 + 10) = 0x23;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x14);
  *(undefined2 *)((int)uVar2 + 0x3e) = 0x28;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x84);
  *(undefined2 *)((int)uVar2 + 6) = 0x27;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x20);
  *(undefined2 *)((int)uVar2 + 0x26) = 0x7e;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0xc);
  *(undefined2 *)((int)uVar2 + 0x16) = 0x7e;
  param_1[0x25] = (iVar1 + 6) * 2;
  param_1[0x27] = 0x50;
  param_1[0x26] = 3;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:61e2  FUN_1010_61e2
   ============================================================ */

void __stdcall16far FUN_1010_61e2(int param_1,undefined2 param_2,int param_3)

{
  undefined2 uVar1;
  int in_DX;
  int iVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 unaff_SS;
  undefined4 uVar5;
  undefined2 *puVar6;
  undefined1 local_10 [4];
  undefined2 local_c;
  undefined2 local_a;
  int local_8;
  int local_6;
  undefined2 local_4;
  
  local_8 = param_3 + -0x27;
  if ((local_8 == 1) && (*(int *)(param_1 + 0x1a) == 0 && *(int *)(param_1 + 0x18) == 0)) {
    local_6 = FUN_1000_1646(0x58);
    if (in_DX == 0 && local_6 == 0) {
      *(undefined2 *)(param_1 + 0x1a) = 0;
      *(undefined2 *)(param_1 + 0x18) = 0;
    }
    else {
      uVar5 = FUN_1010_4f76(local_6,in_DX,1,*(int *)(param_1 + 0x3c) + 1,
                            *(undefined2 *)(param_1 + 4),*(undefined2 *)(param_1 + 6));
      *(undefined2 *)(param_1 + 0x18) = (int)uVar5;
      *(undefined2 *)(param_1 + 0x1a) = (int)((ulong)uVar5 >> 0x10);
    }
    uVar3 = (undefined2)((ulong)*(undefined4 *)(param_1 + 0x18) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 0x18);
    *(int *)(iVar2 + 0xe) = param_1;
    *(undefined2 *)(iVar2 + 0x10) = param_2;
  }
  uVar3 = *(undefined2 *)(param_1 + 0x14 + local_8 * 4);
  uVar1 = *(undefined2 *)(param_1 + 0x14 + local_8 * 4 + 2);
  local_4 = uVar1;
  puVar6 = (undefined2 *)FUN_1008_b946(uVar3,uVar1,0,local_10,unaff_SS);
  local_c = *puVar6;
  local_a = ((undefined2 *)puVar6)[1];
  *(undefined2 *)((int)*(undefined4 *)(param_1 + 4) + 0xea) = local_c;
  *(undefined2 *)((int)*(undefined4 *)(param_1 + 4) + 0xec) = local_a;
  uVar4 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar2 = (int)*(undefined4 *)(param_1 + 4);
  *(undefined2 *)(iVar2 + 0xdc) = uVar3;
  *(undefined2 *)(iVar2 + 0xde) = uVar1;
  FUN_1008_a5fa();
  DAT_1080_1070 = 1;
  FUN_1008_a652(uVar3,uVar1);
  REDRAWWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,0);
  return;
}



/* ============================================================
   1010:6956  FUN_1010_6956
   ============================================================ */

void __stdcall16far FUN_1010_6956(int param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  
  uVar2 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar1 = (int)*(undefined4 *)(param_1 + 4);
  if ((*(int *)(iVar1 + 0xea) == 8) && (*(int *)(iVar1 + 0xec) == 0x13)) {
    uVar2 = 0x69d2;
  }
  else {
    uVar2 = 0x69ee;
  }
  uVar3 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar1 = (int)*(undefined4 *)(param_1 + 4);
  FUN_1008_67e6(*(undefined2 *)(iVar1 + 0xc),*(undefined2 *)(iVar1 + 0xe),uVar2,0x1010);
  return;
}



/* ============================================================
   1010:6a56  FUN_1010_6a56
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_6a56(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined2 uVar6;
  undefined4 local_6;
  
  FUN_1008_7560(param_1,param_2,param_3,param_4,param_5,param_6);
  *param_1 = 0x73e0;
  param_1[1] = 0x1010;
  iVar1 = *(int *)((int)*(undefined4 *)(param_1 + 2) + 0xe4);
  param_1[0x2c] = 1;
  param_1[0x2d] = 1;
  param_1[0x2e] = 1;
  param_1[0x2f] = 1;
  param_1[0x30] = 0;
  local_6 = (undefined2 *)CONCAT22(param_2,param_1);
  FUN_1008_b164(param_1,param_2,0,0x1068);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + DAT_1080_192a * 4);
  *(undefined2 *)((int)uVar2 + DAT_1080_192c * 2) = 0x70;
  uVar2 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)(param_1 + 4) + 4);
  *(undefined2 *)((int)uVar2 + DAT_1080_192c * 2) = 0x71;
  uVar2 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)(param_1 + 4) + 8);
  *(undefined2 *)((int)uVar2 + DAT_1080_192c * 2) = 0x72;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + DAT_1080_192a * 4);
  *(undefined2 *)((int)uVar2 + DAT_1080_192c * 2 + 2) = 0x73;
  uVar2 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)(param_1 + 4) + 4);
  *(undefined2 *)((int)uVar2 + DAT_1080_192c * 2 + 2) = 0x74;
  uVar2 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)(param_1 + 4) + 8);
  *(undefined2 *)((int)uVar2 + DAT_1080_192c * 2 + 2) = 0x75;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + DAT_1080_192a * 4);
  *(undefined2 *)((int)uVar2 + DAT_1080_192c * 2 + 4) = 0x76;
  uVar2 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)(param_1 + 4) + 4);
  *(undefined2 *)((int)uVar2 + DAT_1080_192c * 2 + 4) = 0x77;
  uVar2 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)(param_1 + 4) + 8);
  *(undefined2 *)((int)uVar2 + DAT_1080_192c * 2 + 4) = 0x78;
  uVar6 = 0;
  iVar3 = DAT_1080_192a + -1;
  iVar4 = DAT_1080_192c + -1;
  iVar5 = FUN_1008_3c88(3,iVar4,iVar3,0);
  FUN_1008_80c0(param_1,param_2,0,0,0,iVar5 + iVar1,iVar4,iVar3,uVar6);
  uVar6 = 1;
  iVar3 = DAT_1080_192a + 3;
  iVar4 = DAT_1080_192c + -1;
  iVar5 = FUN_1008_3c88(3,iVar4,iVar3,1);
  FUN_1008_80c0(param_1,param_2,0,0,0,iVar1 + iVar5,iVar4,iVar3,uVar6);
  uVar6 = 0;
  iVar3 = DAT_1080_192a + 3;
  iVar4 = DAT_1080_192c + 3;
  iVar5 = FUN_1008_3c88(3,iVar4,iVar3,0);
  FUN_1008_80c0(param_1,param_2,0,0,0,iVar1 + iVar5,iVar4,iVar3,uVar6);
  uVar6 = 1;
  iVar3 = DAT_1080_192a + -1;
  iVar4 = DAT_1080_192c + 3;
  iVar5 = FUN_1008_3c88(3,iVar4,iVar3,1);
  FUN_1008_80c0(param_1,param_2,0,0,0,iVar1 + iVar5,iVar4,iVar3,uVar6);
  uVar6 = 2;
  iVar3 = DAT_1080_192a + -1;
  iVar4 = DAT_1080_192c + 1;
  iVar5 = FUN_1008_3c88(3,iVar4,iVar3,2);
  FUN_1008_80c0(param_1,param_2,0,0,0,iVar1 + iVar5 + 3,iVar4,iVar3,uVar6);
  uVar6 = 2;
  iVar3 = DAT_1080_192a + 3;
  iVar4 = DAT_1080_192c + 1;
  iVar5 = FUN_1008_3c88(3,iVar4,iVar3,2);
  FUN_1008_80c0(param_1,param_2,0,0,0,iVar1 + iVar5 + 3,iVar4,iVar3,uVar6);
  FUN_1008_c1e6(param_1,param_2,0,0x1f,DAT_1080_192c + -0xb,DAT_1080_192a + -0xb);
  FUN_1008_c1e6(param_1,param_2,0,0x1f,DAT_1080_192c + -0xb,DAT_1080_192a + 0xb);
  FUN_1008_c1e6(param_1,param_2,0,0x1f,DAT_1080_192c + 0xb,DAT_1080_192a + -0xb);
  FUN_1008_c1e6(param_1,param_2,0,0x1f,DAT_1080_192c + 0xb,DAT_1080_192a + 0xb);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x48);
  *(undefined2 *)((int)uVar2 + 0x3c) = 0x43;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x4c);
  *(undefined2 *)((int)uVar2 + 0x3c) = 0x41;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x50);
  *(undefined2 *)((int)uVar2 + 0x3c) = 0x43;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x54);
  *(undefined2 *)((int)uVar2 + 0x3c) = 0x41;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x58);
  *(undefined2 *)((int)uVar2 + 0x3c) = 0x43;
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x50);
  *(undefined2 *)((int)uVar2 + 0x42) = 0x28;
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0x1f,0x12);
  FUN_1008_af5a(param_1[0x1b],param_1[0x1c],3,0x1f,0x16);
  FUN_1008_c1e6(param_1,param_2,0,0x1e,1,0x1e);
  uVar2 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 8);
  *(undefined2 *)((int)uVar2 + 0x36) = 0x27;
  param_1[0x25] = 3;
  param_1[0x26] = 3;
  param_1[0x27] = 0xf;
  param_1[0x31] = 0x19;
  return local_6;
}



/* ============================================================
   1010:6f7a  FUN_1010_6f7a
   ============================================================ */

void __stdcall16far
FUN_1010_6f7a(undefined4 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  bool bVar7;
  int local_a;
  int local_4;
  
  if ((*(int *)((int)param_1 + 0x58) == 1) &&
     (uVar1 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)((int)param_1 + 8) + -0x28),
     *(int *)((int)uVar1 + -0x14 + DAT_1080_192c * 2) != 0x1f)) {
    FUN_1010_7214((int)param_1,param_1._2_2_);
    *(undefined2 *)((int)param_1 + 0x58) = 0;
  }
  if ((*(int *)((int)param_1 + 0x5a) == 1) &&
     (uVar1 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)((int)param_1 + 8) + -0x28),
     *(int *)((int)uVar1 + DAT_1080_192c * 2 + 0x18) != 0x1f)) {
    FUN_1010_7214((int)param_1,param_1._2_2_);
    *(undefined2 *)((int)param_1 + 0x5a) = 0;
  }
  if ((*(int *)((int)param_1 + 0x5c) == 1) &&
     (uVar1 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)((int)param_1 + 8) + 0x30),
     *(int *)((int)uVar1 + -0x14 + DAT_1080_192c * 2) != 0x1f)) {
    FUN_1010_7214((int)param_1,param_1._2_2_);
    *(undefined2 *)((int)param_1 + 0x5c) = 0;
  }
  if ((*(int *)((int)param_1 + 0x5e) == 1) &&
     (uVar1 = *(undefined4 *)(DAT_1080_192a * 4 + (int)*(undefined4 *)((int)param_1 + 8) + 0x30),
     *(int *)((int)uVar1 + DAT_1080_192c * 2 + 0x18) != 0x1f)) {
    FUN_1010_7214((int)param_1,param_1._2_2_);
    *(undefined2 *)((int)param_1 + 0x5e) = 0;
  }
  if ((*(int *)((int)param_1 + 0x62) != 0) &&
     (iVar5 = FUN_1008_3c88(*(undefined2 *)((int)param_1 + 0x62)),
     *(int *)((int)param_1 + 0x62) == iVar5)) {
    for (local_4 = DAT_1080_192a + -10; local_4 <= DAT_1080_192a + 0xc; local_4 = local_4 + 0x16) {
      for (iVar5 = DAT_1080_192c + -10; iVar5 <= DAT_1080_192c + 0xc; iVar5 = iVar5 + 0x16) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + local_4 * 4);
        if (*(int *)((int)uVar1 + iVar5 * 2) == 0x1f) {
          iVar2 = FUN_1008_3c88(4);
          FUN_1008_80c0((int)param_1,param_1._2_2_,0,0,0,1,iVar5,local_4,iVar2 < 2);
        }
      }
    }
  }
  if ((*(int *)((int)param_1 + 0x62) == 0) && (*(int *)((int)param_1 + 0x42) == 0)) {
    *(undefined2 *)((int)param_1 + 0x4a) = 0x19;
    *(undefined2 *)((int)param_1 + 0x4e) = 5;
    *(undefined2 *)((int)param_1 + 0x42) = 1;
    local_a = 200;
    for (iVar5 = 0; iVar5 < 0xc; iVar5 = iVar5 + 1) {
      local_4 = 0;
      do {
        iVar2 = FUN_1008_3c88(0x24);
        iVar3 = FUN_1008_3c88(0x20);
        iVar4 = FUN_1008_ce4e((int)param_1,param_1._2_2_,iVar3 + 2,iVar2 + 2,iVar3,iVar2);
        if (iVar4 == 0) {
          local_4 = FUN_1008_c1e6((int)param_1,param_1._2_2_,0,0x18,iVar3,iVar2);
          iVar4 = local_a + -1;
          bVar7 = local_a < 0;
          local_a = iVar4;
          if (bVar7) break;
        }
      } while (local_4 == 0);
      iVar4 = iVar2;
      if (local_a < 0) break;
      for (; iVar6 = iVar3, iVar4 < iVar2 + 3; iVar4 = iVar4 + 1) {
        for (; iVar6 < iVar3 + 3; iVar6 = iVar6 + 1) {
          FUN_1008_af5a(*(undefined2 *)((int)param_1 + 0x36),*(undefined2 *)((int)param_1 + 0x38),
                        0x16,iVar6,iVar4);
        }
      }
    }
  }
  FUN_1008_ba30((int)param_1,param_1._2_2_,param_2,param_3,param_4);
  return;
}



/* ============================================================
   1010:71f2  FUN_1010_71f2
   ============================================================ */

void __stdcall16far FUN_1010_71f2(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar1 = *(undefined4 *)((int)param_1 + 4);
  uVar3 = (undefined2)((ulong)uVar1 >> 0x10);
  iVar2 = (int)uVar1;
  FUN_1008_67e6(*(undefined2 *)(iVar2 + 0xc),*(undefined2 *)(iVar2 + 0xe),0x7386,0x1010);
  return;
}



/* ============================================================
   1010:7214  FUN_1010_7214
   ============================================================ */

void __stdcall16far FUN_1010_7214(undefined4 param_1)

{
  uint uVar1;
  char cVar2;
  int iVar3;
  undefined2 uVar4;
  
  uVar4 = (undefined2)((ulong)param_1 >> 0x10);
  iVar3 = (int)param_1;
  uVar1 = *(uint *)(iVar3 + 0x62);
  if (uVar1 == 0x19) {
    *(undefined2 *)(iVar3 + 0x62) = 10;
  }
  else if (uVar1 < 0x1a) {
    cVar2 = (char)uVar1;
    if (cVar2 == '\x01') {
      *(undefined2 *)(iVar3 + 0x62) = 0;
    }
    else if (cVar2 == '\x02') {
      *(undefined2 *)(iVar3 + 0x62) = 1;
    }
    else if (cVar2 == '\n') {
      *(undefined2 *)(iVar3 + 0x62) = 2;
    }
  }
  return;
}



/* ============================================================
   1010:746e  FUN_1010_746e
   ============================================================ */

undefined2 __stdcall16far FUN_1010_746e(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  
  if (param_2 == 0) {
    uVar3 = *(undefined2 *)(DAT_1080_1be6 * 4 + 0x192e);
  }
  else {
    uVar3 = *(undefined2 *)(DAT_1080_1be6 * 4 + 0x1930);
    DAT_1080_1be6 = DAT_1080_1be6 + 1;
    if (*(int *)(DAT_1080_1be6 * 4 + 0x1930) == 2) {
      uVar4 = (undefined2)((ulong)param_1 >> 0x10);
      iVar1 = *(int *)((int)param_1 + 4);
      uVar4 = *(undefined2 *)((int)param_1 + 6);
      DAT_1080_2a1e = CONCAT22(uVar4,iVar1);
      FUN_1008_045e(iVar1,uVar4,*(undefined2 *)(iVar1 + 0xd8),*(undefined2 *)(iVar1 + 0xda));
      FUN_1008_169c((undefined2)DAT_1080_2a1e,DAT_1080_2a1e._2_2_);
      FUN_1008_a5fa();
      DAT_1080_1070 = 1;
      uVar4 = (undefined2)((ulong)DAT_1080_2a1e >> 0x10);
      FUN_1008_a652(*(undefined2 *)((int)DAT_1080_2a1e + 0xdc),
                    *(undefined2 *)((int)DAT_1080_2a1e + 0xde));
      uVar2 = *(undefined4 *)((int)DAT_1080_2a1e + 8);
      INVALIDATERECT((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,*(undefined2 *)((int)uVar2 + 4));
      DAT_1080_1be6 = 0;
    }
  }
  return uVar3;
}



/* ============================================================
   1010:757a  FUN_1010_757a
   ============================================================ */

void __stdcall16far FUN_1010_757a(undefined4 param_1)

{
  *(undefined2 *)((int)param_1 + 0xc) = 1;
  return;
}



/* ============================================================
   1010:7642  FUN_1010_7642
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_7642(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  undefined2 uVar1;
  int iVar2;
  undefined2 unaff_CS;
  char *pcVar3;
  
  *param_1 = 0;
  param_1[1] = param_5;
  param_1[2] = param_6;
  param_1[3] = param_3;
  param_1[4] = param_4;
  param_1[0x18] = 0;
  for (iVar2 = 0; iVar2 < 3; iVar2 = iVar2 + 1) {
    param_1[iVar2 + 9] = 0;
  }
  SETRECT(unaff_CS,0x78,0x30,0x60,0x18,param_1 + 0xc);
  SETRECT(0x11d0,0x78,0x84,0x60,0x6c,param_1 + 0x10);
  pcVar3 = (char *)0x11d0;
  SETRECT(0x11d0,0x78,0xd8,0x60,0xc0,param_1 + 0x14);
  if (DAT_1080_1bec != 0 || DAT_1080_1bea != (undefined2 *)0x0) {
    pcVar3 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_2d02(param_1[1],param_1[2],0,0x85aa,0x1010);
  }
  DAT_1080_1bea = param_1;
  DAT_1080_1bec = param_2;
  uVar1 = CREATEFONT(pcVar3,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,700,0,0,0);
  param_1[5] = uVar1;
  uVar1 = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,700,0,0,0);
  param_1[6] = uVar1;
  uVar1 = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,700,0,0,0);
  param_1[7] = uVar1;
  uVar1 = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,400,0,0,0);
  param_1[8] = uVar1;
  FUN_1010_7878(param_1,param_2);
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:77c2  FUN_1010_77c2
   ============================================================ */

void __stdcall16far FUN_1010_77c2(undefined4 param_1)

{
  int iVar1;
  undefined2 uVar2;
  undefined2 unaff_CS;
  undefined2 uVar3;
  undefined2 uVar4;
  
  uVar2 = (undefined2)((ulong)param_1 >> 0x10);
  iVar1 = (int)param_1;
  if (*(int *)(iVar1 + 10) != 0) {
    unaff_CS = 0x11d0;
    DELETEOBJECT();
  }
  uVar3 = unaff_CS;
  if (*(int *)(iVar1 + 0xc) != 0) {
    uVar3 = 0x11d0;
    DELETEOBJECT(unaff_CS,*(undefined2 *)(iVar1 + 0xc));
  }
  uVar4 = uVar3;
  if (*(int *)(iVar1 + 0xe) != 0) {
    uVar4 = 0x11d0;
    DELETEOBJECT(uVar3,*(undefined2 *)(iVar1 + 0xe));
  }
  if (*(int *)(iVar1 + 0x10) != 0) {
    DELETEOBJECT(uVar4,*(undefined2 *)(iVar1 + 0x10));
  }
  return;
}



/* ============================================================
   1010:7816  FUN_1010_7816
   ============================================================ */

bool __stdcall16far
FUN_1010_7816(int *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int iVar1;
  int *piVar2;
  undefined2 unaff_BP;
  undefined2 uVar3;
  undefined2 unaff_CS;
  
  uVar3 = (undefined2)((ulong)param_1 >> 0x10);
  piVar2 = (int *)param_1;
  iVar1 = CREATEWINDOW(unaff_CS,0,0,**(undefined2 **)(piVar2 + 1),1,param_6,param_2,param_3,param_4,
                       param_5,0,0x5000,0,0,0x85d6,0x1010,unaff_BP);
  *param_1 = iVar1;
  if (iVar1 == 0) {
    FUN_1008_2d02(piVar2[1],piVar2[2],0,0x85e6,0x1010);
  }
  return iVar1 != 0;
}



/* ============================================================
   1010:7878  FUN_1010_7878
   ============================================================ */

undefined2 __stdcall16far FUN_1010_7878(undefined4 param_1)

{
  undefined2 unaff_CS;
  undefined2 uVar1;
  undefined2 local_1c;
  undefined2 local_1a;
  undefined2 local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  undefined2 local_c;
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_6;
  undefined2 local_4;
  
  local_1c = 0;
  local_1a = 0x75da;
  local_18 = 0x1010;
  local_16 = 0;
  local_14 = 0;
  local_12 = *(undefined2 *)*(undefined4 *)((int)param_1 + 2);
  local_10 = 0;
  uVar1 = 0;
  local_e = LOADCURSOR(unaff_CS,0x7f00,0);
  local_c = GETSTOCKOBJECT(0x11d0,1,uVar1);
  local_8 = 0;
  local_a = 0;
  local_6 = 0x85d6;
  local_4 = 0x1010;
  REGISTERCLASS(0x11d0,&local_1c);
  return 1;
}



/* ============================================================
   1010:808a  FUN_1010_808a
   ============================================================ */

void __stdcall16far FUN_1010_808a(int *param_1)

{
  undefined4 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  int iVar4;
  int *piVar5;
  undefined2 uVar6;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined1 local_8 [6];
  
  uVar6 = (undefined2)((ulong)param_1 >> 0x10);
  piVar5 = (int *)param_1;
  if (*param_1 != 0) {
    if (piVar5[0x18] < 1) {
      uVar1 = *(undefined4 *)((int)*(undefined4 *)(piVar5 + 1) + 0xdc);
      if (*(int *)((int)uVar1 + 0xc) != 0) {
        DAT_1080_1bee = DAT_1080_1bee + 1;
        if (DAT_1080_1bee == 0x7d) {
          DAT_1080_1bee = 0;
        }
        uVar2 = GETDC();
        FUN_1008_3b6c(local_8,unaff_SS,uVar2);
        uVar3 = FUN_1008_2d50(piVar5[1],piVar5[2]);
        FUN_1008_3c2e(local_8,unaff_SS,(piVar5[9] == 1) + 'f',(piVar5[9] == 1) + '\x1e',uVar3);
        uVar3 = (undefined2)((ulong)*(undefined4 *)(piVar5 + 1) >> 0x10);
        iVar4 = (int)*(undefined4 *)(piVar5 + 1);
        uVar3 = FUN_1008_ced2(*(undefined2 *)(iVar4 + 0xdc),*(undefined2 *)(iVar4 + 0xde));
        FUN_1008_3c2e(local_8,unaff_SS,(piVar5[10] == 1) + 'f',(piVar5[10] == 1) + 'r',uVar3);
        uVar3 = (undefined2)((ulong)*(undefined4 *)(piVar5 + 1) >> 0x10);
        iVar4 = (int)*(undefined4 *)(piVar5 + 1);
        uVar3 = FUN_1008_cf26(*(undefined2 *)(iVar4 + 0xdc),*(undefined2 *)(iVar4 + 0xde));
        FUN_1008_3c2e(local_8,unaff_SS,(piVar5[0xb] == 1) + 'f',(piVar5[0xb] == 1) + -0x3a,uVar3);
        RELEASEDC((char *)s_Lucky_Chip_1080_1004 + 4,uVar2);
        FUN_1008_3ba6(local_8,unaff_SS);
      }
    }
    else {
      piVar5[0x19] = piVar5[0x19] + -1;
      if (piVar5[0x19] < 1) {
        SHOWWINDOW(unaff_CS,0);
        piVar5[0x18] = 0;
      }
    }
  }
  return;
}



/* ============================================================
   1010:81d4  FUN_1010_81d4
   ============================================================ */

void __stdcall16far FUN_1010_81d4(int param_1,undefined2 param_2,int param_3)

{
  undefined2 unaff_CS;
  
  if (param_3 < 1) {
    *(undefined2 *)(param_1 + 0x30) = 0;
    *(undefined2 *)(param_1 + 0x32) = 0;
  }
  else {
    *(int *)(param_1 + 0x30) = param_3;
    *(undefined2 *)(param_1 + 0x32) = 0x20;
    REDRAWWINDOW(unaff_CS,1,0,0,0);
  }
  return;
}



/* ============================================================
   1010:8216  FUN_1010_8216
   ============================================================ */

void __stdcall16far FUN_1010_8216(int param_1,undefined2 param_2,int param_3,undefined2 param_4)

{
  uint uVar1;
  int iVar2;
  undefined2 uVar3;
  undefined2 unaff_SS;
  undefined1 local_10 [6];
  undefined2 local_a;
  undefined2 local_8;
  int local_6;
  int local_4;
  
  if (2 < param_3) {
    return;
  }
  if (param_3 < 0) {
    return;
  }
  if (*(int *)(param_3 * 2 + param_1 + 0x12) == 1) {
    local_a = 0x1b1;
  }
  else {
    local_a = 0x1b0;
  }
  if (param_3 == 0) {
    local_8 = FUN_1008_2d50(*(undefined2 *)(param_1 + 2),*(undefined2 *)(param_1 + 4));
    local_4 = 0x18;
  }
  else if (param_3 == 1) {
    uVar3 = (undefined2)((ulong)*(undefined4 *)(param_1 + 2) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 2);
    local_8 = FUN_1008_ced2(*(undefined2 *)(iVar2 + 0xdc),*(undefined2 *)(iVar2 + 0xde));
    local_4 = 0x6c;
  }
  else {
    if (param_3 != 2) goto LAB_1010_82ab;
    uVar3 = (undefined2)((ulong)*(undefined4 *)(param_1 + 2) >> 0x10);
    iVar2 = (int)*(undefined4 *)(param_1 + 2);
    local_8 = FUN_1008_cf26(*(undefined2 *)(iVar2 + 0xdc),*(undefined2 *)(iVar2 + 0xde));
    local_4 = 0xc0;
  }
  local_6 = 0x60;
LAB_1010_82ab:
  FUN_1008_3b6c(local_10,unaff_SS,param_4);
  FUN_1008_3c2e(local_10,unaff_SS,local_6,local_4,local_a);
  uVar1 = (uint)(*(int *)(param_3 * 2 + param_1 + 0x12) == 1);
  FUN_1008_3c2e(local_10,unaff_SS,uVar1 + local_6 + 6,uVar1 + local_4 + 6,local_8);
  FUN_1008_3ba6(local_10,unaff_SS);
  return;
}



/* ============================================================
   1010:86a0  FUN_1010_86a0
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_86a0(undefined2 *param_1,int param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  undefined2 uVar12;
  
  FUN_1008_7560((undefined2 *)param_1,param_1._2_2_,param_3,param_4,param_5,param_6);
  *param_1 = 0x906c;
  ((undefined2 *)param_1)[1] = 0x1010;
  iVar2 = *(int *)((int)*(undefined4 *)((undefined2 *)param_1 + 2) + 0xe4);
  ((undefined2 *)param_1)[0x2a] = param_2;
  if (param_2 < 3) {
    iVar4 = 2;
  }
  else {
    iVar4 = 1;
  }
  FUN_1008_c5b0((undefined2 *)param_1,param_1._2_2_,0x23,0x27,0,0,iVar4);
  FUN_1008_ce88((undefined2 *)param_1,param_1._2_2_,0,0x19,0x19,0xb,0xb);
  FUN_1008_cafa((undefined2 *)param_1,param_1._2_2_,0xffff,0x17,0x17,0xd,0xd);
  uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x48);
  *(undefined2 *)((int)uVar3 + 0x2e) = 0;
  uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x34);
  *(undefined2 *)((int)uVar3 + 0x24) = 0;
  uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x5c);
  *(undefined2 *)((int)uVar3 + 0x24) = 0;
  uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x48);
  *(undefined2 *)((int)uVar3 + 0x1a) = 0;
  FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,0,0x1a,0xe,0xe);
  FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,0,0x1a,0xe,0x14);
  FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,0,0x1a,0x14,0xe);
  FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,0,0x1a,0x14,0x14);
  for (iVar7 = 0x11; iVar7 < 0x14; iVar7 = iVar7 + 1) {
    FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],1,0x11,iVar7);
    FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],1,0x13,iVar7);
  }
  FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],1,0x12,0x11);
  FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],1,0x12,0x13);
  uVar5 = FUN_1008_3c88(6);
  uVar6 = FUN_1008_3c88(0xe,uVar5);
  FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,1,0x18,uVar6,uVar5);
  uVar5 = FUN_1008_3c88(6);
  iVar7 = FUN_1008_3c88(0xe,uVar5);
  FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,1,0x18,iVar7 + 0x12,uVar5);
  iVar7 = FUN_1008_3c88(9);
  iVar7 = iVar7 + 0x1b;
  uVar5 = FUN_1008_3c88(0xe,iVar7);
  FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,1,0x18,uVar5,iVar7);
  iVar7 = FUN_1008_3c88(9);
  iVar7 = iVar7 + 0x1b;
  iVar8 = FUN_1008_3c88(0xe,iVar7);
  FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,1,0x18,iVar8 + 0x12,iVar7);
  for (iVar7 = 0; iVar7 < (iVar2 + 2) * 2; iVar7 = iVar7 + 1) {
    iVar8 = FUN_1008_3c88(iVar2);
    if (iVar8 < 2) {
      uVar5 = 0;
    }
    else {
      uVar5 = 2;
    }
    uVar6 = FUN_1008_3c88(0x27,uVar5);
    uVar12 = FUN_1008_3c88(0x23,uVar6);
    FUN_1008_80c0((undefined2 *)param_1,param_1._2_2_,0,0,0,1,uVar12,uVar6,uVar5);
  }
  if ((1 < (int)((undefined2 *)param_1)[0x2a]) || (2 < iVar2)) {
    uVar12 = 3;
    uVar5 = FUN_1008_3c88(0x27,3);
    uVar6 = FUN_1008_3c88(0x23,uVar5);
    FUN_1008_80c0((undefined2 *)param_1,param_1._2_2_,0,0,0,1,uVar6,uVar5,uVar12);
  }
  iVar8 = FUN_1008_3c88(3);
  for (iVar7 = 0; iVar7 < iVar8; iVar7 = iVar7 + 1) {
    iVar10 = FUN_1008_3c88((int)(0x28 / (ulong)(long)(iVar4 + 1)));
    iVar11 = FUN_1008_3c88(iVar4);
    iVar11 = iVar10 * (iVar4 + 1) - iVar11;
    iVar10 = FUN_1008_3c88(0xc,iVar11);
    iVar9 = FUN_1008_3c88(iVar4);
    FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],7,
                  iVar10 * (iVar4 + 1) - iVar9,iVar11);
  }
  iVar7 = FUN_1008_3c88(3);
  for (iVar8 = 0; iVar8 < iVar7; iVar8 = iVar8 + 1) {
    iVar10 = FUN_1008_3c88((int)(0x28 / (ulong)(long)(iVar4 + 1)));
    iVar11 = FUN_1008_3c88(iVar4);
    iVar11 = iVar10 * (iVar4 + 1) - iVar11;
    iVar10 = FUN_1008_3c88(0xc,iVar11);
    iVar9 = FUN_1008_3c88(iVar4);
    FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],2,
                  iVar10 * (iVar4 + 1) - iVar9,iVar11);
  }
  for (iVar7 = 0; iVar7 < iVar2; iVar7 = iVar7 + 1) {
    iVar8 = FUN_1008_3c88(0xd);
    iVar8 = iVar8 * 3 + -2;
    iVar10 = FUN_1008_3c88(0xc);
    iVar10 = iVar10 * 3 + -2;
    uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * 4);
    if ((((*(int *)((int)uVar3 + iVar10 * 2) == 0) &&
         (uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * 4 + 4),
         *(int *)((int)uVar3 + iVar10 * 2) == 0)) &&
        (uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * 4),
        *(int *)((int)uVar3 + iVar10 * 2 + 2) == 0)) &&
       (uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * 4 + 4),
       *(int *)((int)uVar3 + iVar10 * 2 + 2) == 0)) {
      FUN_1008_c452((undefined2 *)param_1,param_1._2_2_,iVar10,iVar8);
    }
  }
  do {
    do {
      iVar7 = FUN_1008_3c88(3);
      iVar8 = FUN_1008_3c88(iVar4 + 1);
      iVar8 = iVar8 - iVar7 * (iVar4 + 1);
      iVar7 = -iVar8;
      iVar10 = FUN_1008_3c88(0xb);
      iVar11 = FUN_1008_3c88(iVar4 + 1);
      iVar11 = iVar11 - iVar10 * (iVar4 + 1);
      iVar10 = -iVar11;
      uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * -4);
    } while (*(int *)((int)uVar3 + iVar11 * -2) != 0);
  } while ((((undefined2 *)param_1)[0x2a] == 1) &&
          ((uVar3 = *(undefined4 *)
                     ((int)*(undefined4 *)((undefined2 *)param_1 + 4) + (iVar7 + iVar4) * 4 + 4),
           *(int *)((int)uVar3 + iVar11 * -2) != 0 ||
           (uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * -4),
           *(int *)((int)uVar3 + (iVar10 + iVar4) * 2 + 2) != 0))));
  uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * -4);
  *(undefined2 *)((int)uVar3 + iVar11 * -2) = 0x23;
  if (((undefined2 *)param_1)[0x2a] == 1) {
    uVar3 = *(undefined4 *)
             ((iVar7 + iVar4) * 4 + (int)*(undefined4 *)((undefined2 *)param_1 + 4) + 4);
    *(undefined2 *)((int)uVar3 + iVar11 * -2) = 0x28;
    uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * -4);
    *(undefined2 *)((int)uVar3 + (iVar10 + iVar4) * 2 + 2) = 0x7e;
  }
  do {
    iVar4 = FUN_1008_3c88(3);
    iVar8 = FUN_1008_3c88(2);
    iVar8 = (iVar4 + 10) * 3 - iVar8;
    iVar4 = FUN_1008_3c88(0xc);
    iVar11 = FUN_1008_3c88(2);
    iVar11 = iVar4 * 3 - iVar11;
    uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * 4);
  } while (*(int *)((int)uVar3 + iVar11 * 2) != 0);
  uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * 4);
  *(undefined2 *)((int)uVar3 + iVar11 * 2) = 0x27;
  iVar4 = 0;
  do {
    for (iVar8 = 0; iVar8 < 0x24; iVar8 = iVar8 + 1) {
      uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar4 * 4);
      if ((0x35 < *(int *)((int)uVar3 + iVar8 * 2)) &&
         (uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar4 * 4),
         *(int *)((int)uVar3 + iVar8 * 2) < 0x41)) {
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar4 * 4);
        piVar1 = (int *)((int)uVar3 + iVar8 * 2);
        *piVar1 = *piVar1 + -0xb;
      }
    }
    iVar4 = iVar4 + 1;
  } while (iVar4 < 0x28);
  for (iVar4 = iVar7 + -1; iVar4 <= iVar7 + 1; iVar4 = iVar4 + 1) {
    for (iVar8 = iVar10 + -1; iVar8 <= iVar10 + 1; iVar8 = iVar8 + 1) {
      iVar11 = FUN_1008_91c6((undefined2 *)param_1,param_1._2_2_,iVar8,iVar4);
      if (((iVar11 != 0) &&
          (uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar4 * 4),
          0x2a < *(int *)((int)uVar3 + iVar8 * 2))) &&
         (uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar4 * 4),
         *(int *)((int)uVar3 + iVar8 * 2) < 0x36)) {
        uVar3 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar4 * 4);
        piVar1 = (int *)((int)uVar3 + iVar8 * 2);
        *piVar1 = *piVar1 + 0xb;
        FUN_1008_a060((undefined2 *)param_1,param_1._2_2_,iVar8,iVar4);
      }
    }
  }
  ((undefined2 *)param_1)[0x25] = (iVar2 + ((undefined2 *)param_1)[0x2a]) * 2;
  ((undefined2 *)param_1)[0x27] = (iVar2 + -6) * -10;
  return param_1;
}



/* ============================================================
   1010:8d82  FUN_1010_8d82
   ============================================================ */

void __stdcall16far FUN_1010_8d82(undefined4 param_1,int param_2)

{
  undefined2 uVar1;
  int in_DX;
  int iVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 unaff_SS;
  undefined4 uVar5;
  undefined2 *puVar6;
  undefined1 local_10 [4];
  undefined2 local_c;
  undefined2 local_a;
  int local_8;
  int local_6;
  undefined2 local_4;
  
  local_8 = param_2 + -0x27;
  if (local_8 == 0) {
    if (*(int *)((int)param_1 + 0x16) != 0 || *(int *)((int)param_1 + 0x14) != 0)
    goto LAB_1010_8eb3;
    if (*(int *)((int)param_1 + 0x54) < 3) {
      local_6 = FUN_1000_1646(0x56);
      if (in_DX != 0 || local_6 != 0) {
        uVar5 = FUN_1010_86a0(local_6,in_DX,*(int *)((int)param_1 + 0x54) + 1,0,
                              *(int *)((int)param_1 + 0x3c) + 1,*(undefined2 *)((int)param_1 + 4),
                              *(undefined2 *)((int)param_1 + 6));
        goto LAB_1010_8de8;
      }
LAB_1010_8df5:
      *(undefined2 *)((int)param_1 + 0x16) = 0;
      *(undefined2 *)((int)param_1 + 0x14) = 0;
    }
    else {
      local_6 = FUN_1000_1646(0x5c);
      if (in_DX == 0 && local_6 == 0) goto LAB_1010_8df5;
      uVar5 = FUN_1010_3a96(local_6,in_DX,0,*(int *)((int)param_1 + 0x3c) + 1,
                            *(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6));
LAB_1010_8de8:
      *(undefined2 *)((int)param_1 + 0x14) = (int)uVar5;
      *(undefined2 *)((int)param_1 + 0x16) = (int)((ulong)uVar5 >> 0x10);
    }
    uVar3 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x14) >> 0x10);
    iVar2 = (int)*(undefined4 *)((int)param_1 + 0x14);
  }
  else {
    if ((local_8 != 1) || (*(int *)((int)param_1 + 0x1a) != 0 || *(int *)((int)param_1 + 0x18) != 0)
       ) goto LAB_1010_8eb3;
    local_6 = FUN_1000_1646(0x56);
    if (in_DX == 0 && local_6 == 0) {
      *(undefined2 *)((int)param_1 + 0x1a) = 0;
      *(undefined2 *)((int)param_1 + 0x18) = 0;
    }
    else {
      uVar5 = FUN_1010_cb9c(local_6,in_DX,0,1,*(int *)((int)param_1 + 0x3c) + 1,
                            *(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6));
      *(undefined2 *)((int)param_1 + 0x18) = (int)uVar5;
      *(undefined2 *)((int)param_1 + 0x1a) = (int)((ulong)uVar5 >> 0x10);
    }
    uVar3 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x18) >> 0x10);
    iVar2 = (int)*(undefined4 *)((int)param_1 + 0x18);
  }
  *(int *)(iVar2 + 0xe) = (int)param_1;
  *(undefined2 *)(iVar2 + 0x10) = param_1._2_2_;
LAB_1010_8eb3:
  uVar3 = *(undefined2 *)((int)param_1 + 0x14 + local_8 * 4);
  uVar1 = *(undefined2 *)((int)param_1 + 0x14 + local_8 * 4 + 2);
  local_4 = uVar1;
  puVar6 = (undefined2 *)FUN_1008_b946(uVar3,uVar1,0,local_10,unaff_SS);
  local_c = *puVar6;
  local_a = ((undefined2 *)puVar6)[1];
  *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xea) = local_c;
  *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xec) = local_a;
  uVar4 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 4) >> 0x10);
  iVar2 = (int)*(undefined4 *)((int)param_1 + 4);
  *(undefined2 *)(iVar2 + 0xdc) = uVar3;
  *(undefined2 *)(iVar2 + 0xde) = uVar1;
  FUN_1008_a5fa();
  DAT_1080_1070 = 1;
  FUN_1008_a652(uVar3,uVar1);
  REDRAWWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,0);
  return;
}



/* ============================================================
   1010:8f56  FUN_1010_8f56
   ============================================================ */

void __stdcall16far
FUN_1010_8f56(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined2 uVar7;
  undefined2 local_4;
  
  uVar7 = (undefined2)((ulong)*(undefined4 *)(param_1 + 4) >> 0x10);
  iVar5 = (int)*(undefined4 *)(param_1 + 4);
  iVar2 = *(int *)(iVar5 + 0xea);
  iVar5 = *(int *)(iVar5 + 0xec);
  for (local_4 = iVar2 + -1; local_4 <= iVar2 + 1; local_4 = local_4 + 1) {
    for (iVar6 = iVar5 + -1; iVar6 <= iVar5 + 1; iVar6 = iVar6 + 1) {
      iVar4 = FUN_1008_91c6(param_1,param_2,iVar6,local_4);
      if (((iVar4 != 0) &&
          (uVar3 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + local_4 * 4),
          0x2a < *(int *)((int)uVar3 + iVar6 * 2))) &&
         (uVar3 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + local_4 * 4),
         *(int *)((int)uVar3 + iVar6 * 2) < 0x36)) {
        uVar3 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 8) + local_4 * 4);
        piVar1 = (int *)((int)uVar3 + iVar6 * 2);
        *piVar1 = *piVar1 + 0xb;
        FUN_1008_a060(param_1,param_2,iVar6,local_4);
      }
    }
  }
  FUN_1008_ba30(param_1,param_2,param_3,param_4,param_5);
  return;
}



/* ============================================================
   1010:902e  FUN_1010_902e
   ============================================================ */

void __stdcall16far FUN_1010_902e(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar1 = *(undefined4 *)((int)param_1 + 4);
  uVar3 = (undefined2)((ulong)uVar1 >> 0x10);
  iVar2 = (int)uVar1;
  FUN_1008_67e6(*(undefined2 *)(iVar2 + 0xc),*(undefined2 *)(iVar2 + 0xe),0x905c,0x1010);
  return;
}



/* ============================================================
   1010:90bc  FUN_1010_90bc
   ============================================================ */

int * __stdcall16far
FUN_1010_90bc(int *param_1,undefined2 param_2,undefined2 *param_3,undefined2 param_4,int param_5,
             int param_6,int param_7,int param_8,int param_9)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined2 unaff_CS;
  int local_4;
  
  param_1[7] = param_6;
  param_1[8] = param_7;
  *param_1 = param_9;
  param_1[1] = param_8;
  param_1[2] = param_5;
  SETRECT(unaff_CS,param_3[3],param_3[2],param_3[1],*param_3,param_1 + 3);
  for (iVar3 = 0; iVar3 < 4; iVar3 = iVar3 + 1) {
    param_1[iVar3 * 2 + 10] = 0;
    param_1[iVar3 * 2 + 9] = 0;
  }
  for (local_4 = *param_1; local_4 < param_1[2] + *param_1; local_4 = local_4 + 1) {
    for (iVar3 = param_1[1]; iVar3 < param_1[1] + param_1[2]; iVar3 = iVar3 + 1) {
      iVar2 = FUN_1008_91c6(param_1[7],param_1[8],iVar3,local_4);
      if (iVar2 != 0) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 7) + 8);
        uVar1 = *(undefined4 *)((int)uVar1 + local_4 * 4);
        *(undefined2 *)((int)uVar1 + iVar3 * 2) = 0;
      }
    }
  }
  return (int *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:91a2  FUN_1010_91a2
   ============================================================ */

undefined2 __stdcall16far FUN_1010_91a2(int *param_1)

{
  int iVar1;
  int extraout_DX;
  int iVar2;
  int iVar3;
  undefined2 unaff_SS;
  undefined4 uVar4;
  int local_2c [8];
  int local_1c [10];
  int local_8;
  int local_6;
  int local_4;
  
  local_1c[3] = 1;
  local_1c[5] = 1;
  local_1c[0] = -1;
  local_1c[6] = 0xffff;
  local_1c[1] = 0;
  local_1c[2] = 0;
  local_1c[4] = 0;
  local_1c[7] = 0;
  local_1c[8] = 0;
  for (local_6 = 0; local_6 < 4; local_6 = local_6 + 1) {
    iVar2 = (((int *)param_1)[2] + 1) * local_1c[local_6] + *param_1;
    local_4 = (((int *)param_1)[2] + 1) * local_1c[local_6 + 4] + ((int *)param_1)[1];
    iVar1 = FUN_1008_91c6(((int *)param_1)[7],((int *)param_1)[8],local_4,iVar2);
    iVar3 = local_1c[8];
    if ((((iVar1 != 0) && (((int *)param_1)[3] <= iVar2)) && (iVar2 <= ((int *)param_1)[5])) &&
       (((((int *)param_1)[4] <= local_4 && (local_4 <= ((int *)param_1)[6])) &&
        (uVar4 = *(undefined4 *)((int)*(undefined4 *)((int *)param_1 + 7) + 8),
        uVar4 = *(undefined4 *)((int)uVar4 + iVar2 * 4), *(int *)((int)uVar4 + local_4 * 2) == 3))))
    {
      local_2c[local_1c[8] * 2] = iVar2;
      local_2c[iVar3 * 2 + 1] = local_4;
      local_1c[8] = local_1c[8] + 1;
    }
  }
  for (iVar3 = 0; iVar3 < local_1c[8]; iVar3 = iVar3 + 1) {
    FUN_1010_93d2((int *)param_1,param_1._2_2_,local_2c[iVar3 * 2 + 1],local_2c[iVar3 * 2],
                  ((int *)param_1)[1],*param_1);
    iVar1 = extraout_DX;
    local_1c[9] = FUN_1000_1646(0x22);
    local_8 = iVar1;
    if (iVar1 == 0 && local_1c[9] == 0) {
      ((int *)param_1)[iVar3 * 2 + 10] = 0;
      ((int *)param_1)[iVar3 * 2 + 9] = 0;
    }
    else {
      uVar4 = FUN_1010_90bc(local_1c[9],iVar1,(int *)param_1 + 3,param_1._2_2_,((int *)param_1)[2],
                            ((int *)param_1)[7],((int *)param_1)[8],local_2c[iVar3 * 2 + 1],
                            local_2c[iVar3 * 2]);
      ((int *)param_1)[iVar3 * 2 + 9] = (int)uVar4;
      ((int *)param_1)[iVar3 * 2 + 10] = (int)((ulong)uVar4 >> 0x10);
    }
  }
  if (0 < local_1c[8]) {
    iVar1 = FUN_1008_3c88(local_1c[8]);
    iVar1 = iVar1 + -1;
    local_6 = 0;
    for (iVar3 = 0; iVar3 < local_1c[8]; iVar3 = iVar3 + 1) {
      FUN_1010_91a2(((int *)param_1)[iVar1 * 2 + 9],((int *)param_1 + iVar1 * 2 + 9)[1]);
      iVar1 = iVar1 + 1;
      if (local_1c[8] <= iVar1) {
        iVar1 = 0;
      }
    }
  }
  return 1;
}



/* ============================================================
   1010:9360  FUN_1010_9360
   ============================================================ */

void __stdcall16far FUN_1010_9360(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  for (iVar3 = 0; iVar3 < 4; iVar3 = iVar3 + 1) {
    iVar1 = iVar3 * 4 + param_1;
    if (*(int *)(iVar1 + 0x14) != 0 || *(int *)(iVar1 + 0x12) != 0) {
      iVar2 = iVar3 * 4 + param_1;
      iVar1 = *(int *)(iVar2 + 0x12);
      iVar2 = *(int *)(iVar2 + 0x14);
      if (iVar2 != 0 || iVar1 != 0) {
        FUN_1010_9360(iVar1,iVar2);
        FUN_1000_1634(iVar1,iVar2);
      }
      iVar1 = iVar3 * 4 + param_1;
      *(undefined2 *)(iVar1 + 0x14) = 0;
      *(undefined2 *)(iVar1 + 0x12) = 0;
    }
  }
  return;
}



/* ============================================================
   1010:93d2  FUN_1010_93d2
   ============================================================ */

void __stdcall16far
FUN_1010_93d2(int param_1,undefined2 param_2,int param_3,int param_4,int param_5,int param_6)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  if (param_6 < param_4) {
    iVar2 = 1;
  }
  else if (param_4 == param_6) {
    iVar2 = 0;
  }
  else {
    iVar2 = -1;
  }
  if (param_5 < param_3) {
    iVar3 = 1;
  }
  else if (param_3 == param_5) {
    iVar3 = 0;
  }
  else {
    iVar3 = -1;
  }
  while ((param_4 != param_6 || (param_3 != param_5))) {
    param_6 = param_6 + iVar2;
    param_5 = param_5 + iVar3;
    if (iVar2 == 0) {
      iVar5 = 0;
      while ((iVar5 < *(int *)(param_1 + 4) &&
             (iVar4 = FUN_1008_91c6(*(undefined2 *)(param_1 + 0xe),*(undefined2 *)(param_1 + 0x10),
                                    param_5,param_6 + iVar5), iVar4 != 0))) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 0xe) + 8);
        uVar1 = *(undefined4 *)((int)uVar1 + (param_6 + iVar5) * 4);
        iVar5 = iVar5 + 1;
        *(undefined2 *)((int)uVar1 + param_5 * 2) = 0;
      }
    }
    else if (iVar3 == 0) {
      iVar5 = 0;
      while ((iVar5 < *(int *)(param_1 + 4) &&
             (iVar4 = FUN_1008_91c6(*(undefined2 *)(param_1 + 0xe),*(undefined2 *)(param_1 + 0x10),
                                    param_5 + iVar5,param_6), iVar4 != 0))) {
        iVar4 = param_5 + iVar5;
        uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 0xe) + 8);
        uVar1 = *(undefined4 *)((int)uVar1 + param_6 * 4);
        iVar5 = iVar5 + 1;
        *(undefined2 *)((int)uVar1 + iVar4 * 2) = 0;
      }
    }
  }
  return;
}



/* ============================================================
   1010:94e0  FUN_1010_94e0
   ============================================================ */

undefined2 __stdcall16far
FUN_1010_94e0(undefined2 param_1,undefined2 param_2,undefined2 param_3,int param_4,
             undefined2 param_5)

{
  undefined2 uVar1;
  undefined2 unaff_SS;
  
  if (param_4 == 2) {
    uVar1 = FUN_1010_9744(*(undefined2 *)&DAT_1080_1bf2,*(undefined2 *)&DAT_1080_1bf4);
  }
  else if (param_4 == 0xf) {
    uVar1 = FUN_1010_99ba(*(undefined2 *)&DAT_1080_1bf2,*(undefined2 *)&DAT_1080_1bf4);
  }
  else if (param_4 == 0x10) {
    uVar1 = FUN_1010_975c(*(undefined2 *)&DAT_1080_1bf2,*(undefined2 *)&DAT_1080_1bf4);
  }
  else if (param_4 == 0x110) {
    uVar1 = FUN_1010_97f2(*(undefined2 *)&DAT_1080_1bf2,*(undefined2 *)&DAT_1080_1bf4,param_5);
  }
  else if (param_4 == 0x111) {
    uVar1 = FUN_1010_9774(*(undefined2 *)&DAT_1080_1bf2,*(undefined2 *)&DAT_1080_1bf4,param_3);
  }
  else if (param_4 == 0x201) {
    uVar1 = FUN_1010_ac1c(*(undefined2 *)&DAT_1080_1bf2,*(undefined2 *)&DAT_1080_1bf4,param_1,
                          param_2);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



/* ============================================================
   1010:9572  FUN_1010_9572
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_9572(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  undefined2 uVar1;
  undefined2 unaff_CS;
  
  DAT_1080_1bf2 = param_1;
  DAT_1080_1bf4 = param_2;
  *param_1 = param_5;
  param_1[1] = param_6;
  param_1[2] = param_4;
  param_1[4] = param_3;
  param_1[3] = 0;
  uVar1 = LOADICON(unaff_CS,0x748e,(char *)s_Lucky_Chip_1080_1004 + 4);
  param_1[5] = uVar1;
  uVar1 = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,700,0,0,0);
  param_1[6] = uVar1;
  uVar1 = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,700,0,0,0);
  param_1[7] = uVar1;
  uVar1 = CREATEFONT(0x11d0,0xb9e2,0x1010,0x11,0,0,5,0,0,1,0,700,0,0,0);
  param_1[8] = uVar1;
  uVar1 = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,400,0,0,0);
  param_1[9] = uVar1;
  param_1[10] = 0;
  SETRECT(0x11d0,0x18c,0x96,0x174,0x7e,param_1 + 0xb);
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:9694  FUN_1010_9694
   ============================================================ */

void __stdcall16far FUN_1010_9694(undefined4 param_1)

{
  int iVar1;
  undefined2 uVar2;
  undefined2 unaff_CS;
  undefined2 uVar3;
  undefined2 uVar4;
  
  uVar2 = (undefined2)((ulong)param_1 >> 0x10);
  iVar1 = (int)param_1;
  if (*(int *)(iVar1 + 10) != 0) {
    unaff_CS = 0x11d0;
    DESTROYICON();
  }
  uVar3 = unaff_CS;
  if (*(int *)(iVar1 + 0xc) != 0) {
    uVar3 = 0x11d0;
    DELETEOBJECT(unaff_CS,*(undefined2 *)(iVar1 + 0xc));
  }
  uVar4 = uVar3;
  if (*(int *)(iVar1 + 0xe) != 0) {
    uVar4 = 0x11d0;
    DELETEOBJECT(uVar3,*(undefined2 *)(iVar1 + 0xe));
  }
  uVar3 = uVar4;
  if (*(int *)(iVar1 + 0x10) != 0) {
    uVar3 = 0x11d0;
    DELETEOBJECT(uVar4,*(undefined2 *)(iVar1 + 0x10));
  }
  if (*(int *)(iVar1 + 0x12) != 0) {
    DELETEOBJECT(uVar3,*(undefined2 *)(iVar1 + 0x12));
  }
  return;
}



/* ============================================================
   1010:96fa  FUN_1010_96fa
   ============================================================ */

void __stdcall16far FUN_1010_96fa(int param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined4 uVar1;
  
  uVar1 = MAKEPROCINSTANCE(unaff_CS,*(undefined2 *)(param_1 + 8),0x94e0);
  DIALOGBOX(0x11d0,uVar1,*(undefined2 *)(param_1 + 4),0xb9f2,0x1010);
  FREEPROCINSTANCE(0x11d0,(int)uVar1);
  return;
}



/* ============================================================
   1010:9744  FUN_1010_9744
   ============================================================ */

undefined2 __stdcall16far FUN_1010_9744(void)

{
  undefined2 unaff_CS;
  
  ENDDIALOG(unaff_CS,0);
  return 0;
}



/* ============================================================
   1010:975c  FUN_1010_975c
   ============================================================ */

undefined2 __stdcall16far FUN_1010_975c(void)

{
  undefined2 unaff_CS;
  
  ENDDIALOG(unaff_CS,0);
  return 0;
}



/* ============================================================
   1010:9774  FUN_1010_9774
   ============================================================ */

undefined2 __stdcall16far FUN_1010_9774(undefined4 param_1,uint param_2)

{
  char cVar1;
  undefined2 unaff_CS;
  int iVar2;
  
  if (param_2 == 100) {
    ENDDIALOG(unaff_CS,0);
  }
  else {
    if (100 < param_2) {
      return 0;
    }
    cVar1 = (char)param_2;
    if (cVar1 == '\x02') {
      iVar2 = 0xb;
    }
    else if (cVar1 == '\x03') {
      iVar2 = 1;
    }
    else if (cVar1 == '\x04') {
      iVar2 = DAT_1080_1bf0 + -1;
    }
    else if (cVar1 == '\x05') {
      iVar2 = DAT_1080_1bf0 + 1;
    }
    else if (cVar1 == '\x06') {
      iVar2 = 0xc;
    }
    else if (cVar1 == '\b') {
      iVar2 = 10;
    }
    else if (cVar1 == '\t') {
      iVar2 = 4;
    }
    else if (cVar1 == '\n') {
      iVar2 = 6;
    }
    else {
      if (cVar1 != '\v') {
        return 0;
      }
      iVar2 = 8;
    }
    FUN_1010_9c6c((undefined2)param_1,param_1._2_2_,iVar2);
  }
  return 1;
}



/* ============================================================
   1010:97f2  FUN_1010_97f2
   ============================================================ */

undefined2 __stdcall16far FUN_1010_97f2(int param_1,undefined2 param_2,undefined2 param_3)

{
  int iVar1;
  undefined2 uVar2;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 local_a;
  undefined2 local_8;
  int local_6;
  int local_4;
  
  *(undefined2 *)(param_1 + 6) = param_3;
  GETWINDOWRECT(unaff_CS,&local_a,unaff_SS);
  uVar5 = 0x1e0;
  uVar4 = 4;
  uVar3 = 0x11d0;
  iVar1 = GETSYSTEMMETRICS(0x11d0,4,0x1e0,local_8,local_a,*(undefined2 *)(param_1 + 6),param_3);
  MOVEWINDOW(0x11d0,1,iVar1 + 0x1b0,uVar3,uVar4,uVar5);
  uVar3 = *(undefined2 *)(param_1 + 6);
  uVar5 = GETDLGITEM(0x11d0,100);
  MOVEWINDOW(0x11d0,1,0x12,0x60,0x36,0x168);
  uVar4 = *(undefined2 *)(param_1 + 6);
  uVar2 = GETDLGITEM(0x11d0,100);
  SETFOCUS(0x11d0,uVar2,uVar4,uVar5,uVar3);
  GETDLGITEM(0x11d0,2);
  MOVEWINDOW(0x11d0,1,0x12,0x6c,0x36,0xf0);
  GETDLGITEM(0x11d0,10);
  MOVEWINDOW(0x11d0,1,0x12,0x60,6,0x168);
  GETDLGITEM(0x11d0,0xb);
  MOVEWINDOW(0x11d0,1,0x12,0x6c,6,0xf0);
  GETDLGITEM(0x11d0,8);
  MOVEWINDOW(0x11d0,1,0x12,0x6c,0x1e,0xf0);
  GETDLGITEM(0x11d0,9);
  MOVEWINDOW(0x11d0,1,0x12,0x60,0x1e,0x168);
  GETDLGITEM(0x11d0,3);
  MOVEWINDOW(0x11d0,1,0x18,0x18,0x24,0x48);
  GETDLGITEM(0x11d0,4);
  MOVEWINDOW(0x11d0,1,0x18,0x18,0x24,0x6c);
  GETDLGITEM(0x11d0,5);
  MOVEWINDOW(0x11d0,1,0x18,0x18,0x24,0x90);
  GETDLGITEM(0x11d0,6);
  MOVEWINDOW(0x11d0,1,0x18,0x18,0x24,0xb4);
  GETCLIENTRECT(0x11d0,&local_a,unaff_SS);
  GETDLGITEM(0x11d0,7);
  MOVEWINDOW(0x11d0,1,local_4 + -0x60,local_6 + -0x18,0x54,0xc);
  return 0;
}



/* ============================================================
   1010:99ba  FUN_1010_99ba
   ============================================================ */

undefined2 __stdcall16far FUN_1010_99ba(int param_1,undefined2 param_2)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 local_3c [16];
  undefined1 local_1c [16];
  undefined1 local_c [4];
  undefined2 local_8;
  undefined2 local_6;
  undefined2 local_4;
  
  BEGINPAINT(unaff_CS,local_3c,unaff_SS,*(undefined2 *)(param_1 + 6));
  uVar1 = GETSTOCKOBJECT(0x11d0,1,local_3c[0]);
  SELECTOBJECT(0x11d0,uVar1);
  GETCLIENTRECT(0x11d0,local_c,unaff_SS);
  RECTANGLE(0x11d0,local_6,local_8,0,0);
  DRAWICON(0x11d0,*(undefined2 *)(param_1 + 10),0x18,0xc);
  uVar2 = CREATEFONT(0x11d0,0x32d8,(char *)s_Lucky_Chip_1080_1004 + 4,0x31,0,0,5,0,0,0,0,700,0,0,0);
  local_4 = SELECTOBJECT(0x11d0,uVar2);
  uVar1 = local_3c[0];
  SETTEXTCOLOR(0x11d0,0x80,0);
  FUN_1000_1924(local_1c);
  FUN_1008_3d50(local_3c[0],0x48,0xc,local_1c);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,local_4);
  DELETEOBJECT(0x11d0,uVar2,local_3c[0],uVar1);
  ENDPAINT(0x11d0,local_3c,unaff_SS);
  FUN_1010_9ac4(param_1,param_2);
  return 0;
}



/* ============================================================
   1010:9ac4  FUN_1010_9ac4
   ============================================================ */

void __stdcall16far FUN_1010_9ac4(undefined4 param_1)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar6;
  undefined1 local_e [4];
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_6;
  undefined2 local_4;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  uVar3 = *(undefined2 *)(iVar4 + 6);
  uVar1 = GETDLGITEM(unaff_CS,7);
  uVar6 = uVar1;
  GETCLIENTRECT(0x11d0,local_e,unaff_SS);
  uVar2 = GETDC(0x11d0,uVar1,uVar6,uVar3);
  uVar3 = GETSTOCKOBJECT(0x11d0,1,uVar2);
  SELECTOBJECT(0x11d0,uVar3);
  uVar3 = GETSTOCKOBJECT(0x11d0,8,uVar2);
  SELECTOBJECT(0x11d0,uVar3);
  RECTANGLE(0x11d0,local_8,local_a,0,0);
  local_4 = CREATEPEN(0x11d0,0x8080,0x80,1);
  local_6 = SELECTOBJECT(0x11d0,local_4);
  uVar1 = uVar2;
  MOVETO(0x11d0,local_8,0);
  uVar6 = uVar2;
  LINETO(0x11d0,0,0);
  uVar3 = uVar2;
  LINETO(0x11d0,0,local_a);
  uVar3 = GETSTOCKOBJECT(0x11d0,6,uVar2,uVar3,uVar6,uVar1);
  SELECTOBJECT(0x11d0,uVar3);
  uVar6 = uVar2;
  LINETO(0x11d0,local_8,local_a);
  uVar1 = 0x11d0;
  uVar3 = uVar2;
  LINETO(0x11d0,local_8,0);
  switch(DAT_1080_1bf0) {
  case 1:
    uVar1 = 0x1010;
    FUN_1010_a466(iVar4,uVar5,uVar2);
    break;
  case 2:
    uVar1 = 0x1010;
    FUN_1010_a4d8(iVar4,uVar5,uVar2);
    break;
  case 3:
    uVar1 = 0x1010;
    FUN_1010_a582(iVar4,uVar5,uVar2);
    break;
  case 4:
    uVar1 = 0x1010;
    FUN_1010_a16e(iVar4,uVar5,uVar2);
    break;
  case 5:
    uVar1 = 0x1010;
    FUN_1010_b638(iVar4,uVar5,uVar2);
    break;
  case 6:
    uVar1 = 0x1010;
    FUN_1010_9cb2(iVar4,uVar5,uVar2);
    break;
  case 7:
    uVar1 = 0x1010;
    FUN_1010_9eec(iVar4,uVar5,uVar2);
    break;
  case 8:
    uVar1 = 0x1010;
    FUN_1010_a966(iVar4,uVar5,uVar2);
    break;
  case 9:
    uVar1 = 0x1010;
    FUN_1010_a9ee(iVar4,uVar5,uVar2);
    break;
  case 10:
    uVar1 = 0x1010;
    FUN_1010_a64a(iVar4,uVar5,uVar2);
    break;
  case 0xb:
    uVar1 = 0x1010;
    FUN_1010_aa76(iVar4,uVar5,uVar2);
    break;
  case 0xc:
    uVar1 = 0x1010;
    FUN_1010_b4ae(iVar4,uVar5,uVar2);
  }
  uVar5 = uVar2;
  SELECTOBJECT(uVar1,local_6);
  DELETEOBJECT(0x11d0,local_4,uVar5,uVar3,uVar6);
  RELEASEDC(0x11d0,uVar2);
  return;
}



/* ============================================================
   1010:9c6c  FUN_1010_9c6c
   ============================================================ */

void __stdcall16far FUN_1010_9c6c(undefined4 param_1,int param_2)

{
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined1 local_a [8];
  
  if (param_2 < 1) {
    param_2 = 1;
  }
  if (0xc < param_2) {
    param_2 = 0xc;
  }
  DAT_1080_1bf0 = param_2;
  SETRECT(unaff_CS,0x24,0xe4,0,0x48,local_a);
  INVALIDATERECT(0x11d0,1,local_a,unaff_SS,*(undefined2 *)((int)param_1 + 6));
  return;
}



/* ============================================================
   1010:9cb2  FUN_1010_9cb2
   ============================================================ */

void __stdcall16far FUN_1010_9cb2(undefined4 param_1,undefined2 param_2)

{
  undefined2 *puVar1;
  long lVar2;
  int iVar3;
  int extraout_DX;
  int iVar4;
  int iVar5;
  undefined2 *puVar6;
  undefined2 uVar7;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar8;
  undefined2 uVar9;
  undefined2 uVar10;
  undefined2 local_14;
  undefined2 local_12 [2];
  undefined2 local_e;
  long local_c;
  undefined4 local_8;
  int local_4;
  
  uVar7 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  uVar10 = param_2;
  local_e = SELECTOBJECT(unaff_CS,*(undefined2 *)(iVar4 + 0xe));
  uVar9 = param_2;
  SETTEXTCOLOR(0x11d0,0,0x80);
  uVar8 = param_2;
  SETBKMODE(0x11d0,1);
  iVar3 = extraout_DX;
  local_8._0_2_ = FUN_1000_1646(6,uVar8,uVar9,uVar10);
  local_8._2_2_ = iVar3;
  if (iVar3 == 0 && (int)local_8 == 0) {
    local_c = 0;
  }
  else {
    local_c = FUN_1008_3b6c((int)local_8,iVar3,param_2);
  }
  local_14 = *(undefined2 *)0xba0c;
  puVar6 = local_12;
  for (iVar3 = 2; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar1 = puVar6;
    puVar6 = puVar6 + 1;
    *puVar1 = 0;
  }
  local_4 = 1;
  lVar2 = local_c;
  for (iVar3 = 1; iVar3 < 4; iVar3 = iVar3 + 1) {
    local_4 = iVar3;
    for (iVar5 = 1; iVar5 < 4; iVar5 = iVar5 + 1) {
      if ((iVar3 != 2) || (iVar5 != 2)) {
        local_c = lVar2;
        FUN_1008_3bd0(lVar2,(iVar5 + 2) * 2,iVar3 * 2 + 0x19,0x1a5);
        local_14 = CONCAT11(local_14._1_1_,('\x13' - (char)iVar5) * '\x03' + (char)iVar3);
        TEXTOUT((char *)s_Lucky_Chip_1080_1004 + 4,1,&local_14,unaff_SS,(iVar5 + 2) * 0x18,
                iVar3 * 0x18 + 0x132);
        FUN_1008_3c2e(local_c,iVar5 * 0x2a + 0x12,iVar3 * 0x2a + 0x10e,iVar5 * 3 + iVar3 + 0x1a3);
        lVar2 = local_c;
      }
    }
  }
  local_c = lVar2;
  FUN_1008_3bd0(lVar2,8,7,0x1a5);
  local_14._0_1_ = 0x41;
  TEXTOUT((char *)s_Lucky_Chip_1080_1004 + 4,1,&local_14,unaff_SS,0x60,0x5a);
  FUN_1008_3bd0(lVar2,10,8,0x1a5);
  local_14 = CONCAT11(local_14._1_1_,0x5a);
  TEXTOUT((char *)s_Lucky_Chip_1080_1004 + 4,1,&local_14,unaff_SS,0x78,0x66);
  FUN_1008_3bd0(lVar2,10,4,0x1a6);
  FUN_1000_16aa(&local_14);
  TEXTOUT(0x1000,5,&local_14,unaff_SS,0x78,0x36);
  FUN_1010_ae4a(iVar4,uVar7,0,0xff,0xa8,0x1e0,0x3c,0x60,0xba14,0x1010,*(undefined2 *)(iVar4 + 0xe),
                param_2);
  FUN_1010_ae4a(iVar4,uVar7,0x80,0,0x138,0x1e0,0xc0,0xc,0xba2e,0x1010,*(undefined2 *)(iVar4 + 0xe),
                param_2);
  uVar9 = param_2;
  SELECTOBJECT(0x1010,*(undefined2 *)(iVar4 + 0x10));
  uVar8 = param_2;
  SETTEXTCOLOR(0x11d0,0,0x80);
  FUN_1008_3d50(param_2,6,6,0xbae6,0x1010,uVar8,uVar9);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,local_e);
  if (lVar2 != 0) {
    local_8 = lVar2;
    FUN_1008_3ba6(lVar2);
    FUN_1000_1634(local_8,param_2);
  }
  return;
}



/* ============================================================
   1010:9eec  FUN_1010_9eec
   ============================================================ */

void __stdcall16far FUN_1010_9eec(undefined4 param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 uVar2;
  int extraout_DX;
  int iVar3;
  undefined2 uVar4;
  undefined2 unaff_CS;
  char *pcVar5;
  undefined2 unaff_SS;
  undefined1 *puVar6;
  undefined2 uVar7;
  undefined1 local_72 [100];
  undefined2 local_e;
  undefined1 local_c [2];
  int local_a;
  undefined4 local_8;
  undefined2 local_4;
  
  uVar4 = (undefined2)((ulong)param_1 >> 0x10);
  uVar2 = param_2;
  local_e = SELECTOBJECT(unaff_CS,*(undefined2 *)((int)param_1 + 0x10));
  uVar7 = param_2;
  SETTEXTCOLOR(0x11d0,0x60,0x60);
  FUN_1008_3d50(param_2,0xc,0xc,0xbaf8,0x1010,uVar7,uVar2);
  uVar2 = param_2;
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)((int)param_1 + 0xe));
  uVar7 = param_2;
  SETBKMODE(0x11d0,1);
  pcVar5 = (char *)0x1000;
  iVar3 = extraout_DX;
  iVar1 = FUN_1000_1646(6,uVar7,uVar2);
  local_a = iVar3;
  if (iVar3 == 0 && iVar1 == 0) {
    local_8 = 0;
  }
  else {
    pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
    local_8 = FUN_1008_3b6c(iVar1,iVar3,param_2);
  }
  local_4 = 1;
  iVar3 = 1;
  do {
    iVar1 = (int)local_8;
    if (9 < iVar3) {
      SELECTOBJECT(pcVar5,local_e);
      iVar3 = local_8._2_2_;
      local_a = local_8._2_2_;
      if (local_8._2_2_ != 0 || iVar1 != 0) {
        FUN_1008_3ba6(iVar1,local_8._2_2_);
        FUN_1000_1634(iVar1,iVar3,param_2);
      }
      return;
    }
    pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_3c2e(local_8,iVar3 * 0x1e + 0x12,0xc,0x1a5);
    switch(iVar3) {
    case 1:
      FUN_1000_16aa(local_c);
      break;
    case 2:
      FUN_1000_16aa(local_c);
      break;
    case 3:
      FUN_1000_16aa(local_c);
      break;
    case 4:
      FUN_1000_16aa(local_c);
      break;
    case 5:
      FUN_1000_16aa(local_c);
      FUN_1000_16aa(local_72);
      FUN_1000_1656(local_72);
      goto LAB_1010_a0d4;
    case 6:
      FUN_1000_16aa(local_c);
      break;
    case 7:
      FUN_1000_16aa(local_c);
      break;
    case 8:
      FUN_1000_16aa(local_c);
      break;
    case 9:
      FUN_1000_16aa(local_c);
      break;
    default:
      goto switchD_1010_9f8b_default;
    }
    FUN_1000_16aa(local_72);
LAB_1010_a0d4:
    pcVar5 = (char *)0x1000;
switchD_1010_9f8b_default:
    SETTEXTCOLOR(pcVar5,0,0x80);
    uVar4 = 0x12;
    iVar1 = iVar3 * 0x1e + 0x12;
    puVar6 = local_c;
    uVar7 = unaff_SS;
    uVar2 = FUN_1000_1710(puVar6);
    TEXTOUT(0x1000,uVar2,puVar6,uVar7,iVar1,uVar4);
    SETTEXTCOLOR(0x11d0,0x60,0);
    uVar4 = 0x3c;
    iVar1 = iVar3 * 0x1e + 0x18;
    puVar6 = local_72;
    uVar7 = unaff_SS;
    uVar2 = FUN_1000_1710(puVar6);
    pcVar5 = (char *)0x11d0;
    TEXTOUT(0x1000,uVar2,puVar6,uVar7,iVar1,uVar4);
    iVar3 = iVar3 + 1;
  } while( true );
}



/* ============================================================
   1010:a16e  FUN_1010_a16e
   ============================================================ */

void __stdcall16far FUN_1010_a16e(int param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  undefined2 unaff_SS;
  undefined2 uVar2;
  undefined1 local_a [6];
  undefined2 local_4;
  
  FUN_1008_3b6c(local_a,unaff_SS,param_3);
  uVar2 = param_3;
  local_4 = SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,*(undefined2 *)(param_1 + 0x10));
  uVar1 = param_3;
  SETTEXTCOLOR(0x11d0,0xc0c0,0xc0);
  FUN_1008_3d50(param_3,0xc,0xc,0xbc1c,0x1010,uVar1,uVar2);
  uVar1 = param_3;
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,local_4);
  uVar1 = GETSTOCKOBJECT(0x11d0,5,param_3,uVar1);
  SELECTOBJECT(0x11d0,uVar1);
  uVar1 = GETSTOCKOBJECT(0x11d0,7,param_3);
  SELECTOBJECT(0x11d0,uVar1);
  FUN_1010_ae4a(param_1,param_2,0x80,0,0xa8,0xd8,0x30,0xc,0xbc36,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  RECTANGLE(0x1010,0x4e,0xd2,0x36,0xba);
  FUN_1010_ae4a(param_1,param_2,0,0x80,0x90,0x1b0,0x30,300,0xbc70,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  RECTANGLE(0x1010,0x42,0x1aa,0x2a,0x192);
  FUN_1010_ae4a(param_1,param_2,0x8000,0,0xd8,0xb4,0xa8,0xc,0xbc9c,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  RECTANGLE(0x1010,0xa2,0x66,0x8a,0x36);
  FUN_1008_3bd0(local_a,unaff_SS,0xb,0x10,0x69);
  FUN_1008_3bd0(local_a,unaff_SS,0xb,0x11,0x6a);
  FUN_1008_3bd0(local_a,unaff_SS,0xb,0x12,0x6b);
  FUN_1008_3bd0(local_a,unaff_SS,0xc,0x10,0x6c);
  FUN_1008_3bd0(local_a,unaff_SS,0xc,0x11,0x6e);
  FUN_1008_3bd0(local_a,unaff_SS,0xc,0x12,0x74);
  FUN_1008_3bd0(local_a,unaff_SS,0xd,0x10,0x75);
  FUN_1008_3bd0(local_a,unaff_SS,0xd,0x11,0x76);
  FUN_1008_3bd0(local_a,unaff_SS,0xd,0x12,0x77);
  FUN_1010_ae4a(param_1,param_2,0x80,0,0xcc,0x1b0,0x84,0xf0,0xbcc2,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1008_3bd0(local_a,unaff_SS,0x10,0x1c,0x25);
  FUN_1008_3bd0(local_a,unaff_SS,0x10,0x1d,0x25);
  FUN_1008_3bd0(local_a,unaff_SS,0x11,0x1d,0x25);
  FUN_1008_3bd0(local_a,unaff_SS,0x11,0x1e,0x25);
  FUN_1010_ae4a(param_1,param_2,0x8000,0,0xfc,0x198,0xcc,0x108,0xbcfe,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1008_3bd0(local_a,unaff_SS,0x19,8,0x46);
  FUN_1008_3bd0(local_a,unaff_SS,0x19,9,0);
  FUN_1008_3bd0(local_a,unaff_SS,0x19,10,0x57);
  FUN_1008_3bd0(local_a,unaff_SS,0x19,0xb,0);
  FUN_1008_3bd0(local_a,unaff_SS,0x19,0xc,0x46);
  RECTANGLE((char *)s_Lucky_Chip_1080_1004 + 4,0x13e,0xa2,0x126,0x5a);
  FUN_1010_ae4a(param_1,param_2,0,0x80,0x138,0xd8,0xf0,0x18,0xbd1a,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1010_ae4a(param_1,param_2,0x60,0x60,0x14a,0x1b0,0x10e,0xf0,0xbd5a,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  RECTANGLE(0x1010,0x138,0x1b0,0x108,0xe4);
  FUN_1008_3ba6(local_a,unaff_SS);
  return;
}



/* ============================================================
   1010:a466  FUN_1010_a466
   ============================================================ */

void __stdcall16far FUN_1010_a466(int param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  
  FUN_1010_ae4a(param_1,param_2,0x80,0,0x1e0,0x1b0,0xc,0xc,
                (char *)s_IN_THE_MURKY_CHAMBERS_beneath_yo_1080_1bf6,0x1080,
                *(undefined2 *)(param_1 + 0xe),param_3);
  uVar3 = param_3;
  uVar1 = SELECTOBJECT(0x1010,*(undefined2 *)(param_1 + 0xc));
  uVar2 = param_3;
  SETTEXTCOLOR(0x11d0,0,0xff);
  FUN_1008_3d50(param_3,0xc,6,0xbd88,0x1010,uVar2,uVar3);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,uVar1);
  return;
}



/* ============================================================
   1010:a4d8  FUN_1010_a4d8
   ============================================================ */

void __stdcall16far FUN_1010_a4d8(int param_1,undefined2 param_2,undefined2 param_3)

{
  int iVar1;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined1 local_c [6];
  undefined2 local_6;
  undefined2 local_4;
  
  uVar3 = param_3;
  local_6 = SELECTOBJECT(unaff_CS,*(undefined2 *)(param_1 + 0xc));
  uVar2 = param_3;
  SETTEXTCOLOR(0x11d0,0xc0c0,0xc0);
  FUN_1008_3d50(param_3,0xc,0x12,0xbda0,0x1010,uVar2,uVar3);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,local_6);
  FUN_1010_ae4a(param_1,param_2,0x80,0,0x168,0x1b0,0x30,0xc,
                (char *)s_This_mighty_drain_probe_is_named_1080_1d4a,0x1080,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1008_3b6c(local_c,unaff_SS,param_3);
  local_4 = 0xd;
  for (iVar1 = 0xd; iVar1 < 0x14; iVar1 = iVar1 + 1) {
    FUN_1008_3c2e(local_c,unaff_SS,0x5a,iVar1 * 0xc,0);
  }
  FUN_1008_3ba6(local_c,unaff_SS);
  return;
}



/* ============================================================
   1010:a582  FUN_1010_a582
   ============================================================ */

void __stdcall16far FUN_1010_a582(int param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 unaff_CS;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  
  uVar5 = param_3;
  uVar1 = SELECTOBJECT(unaff_CS,*(undefined2 *)(param_1 + 0xc));
  uVar3 = param_3;
  SETTEXTCOLOR(0x11d0,0xc0c0,0xc0);
  FUN_1008_3d50(param_3,0xc,0xc,0xbda0,0x1010,uVar3,uVar5);
  FUN_1010_ae4a(param_1,param_2,0x80,0,0x168,0x1b0,0x30,0xc,
                (char *)s_Once_you_ve_mastered_the_art_of_s_1080_1f02,0x1080,
                *(undefined2 *)(param_1 + 0xe),param_3);
  uVar4 = 0xffd0;
  uVar5 = param_3;
  uVar2 = CREATEFONT(0x1010,0xb9e2,0x1010,0x11,0,0,5,0,0,0,0,700,0,0,0);
  SELECTOBJECT(0x11d0,uVar2);
  uVar3 = param_3;
  SETTEXTCOLOR(0x11d0,0x60,0x60);
  FUN_1008_3d50(param_3,0x60,0xfc,0xbdae,0x1010,uVar3,uVar4,uVar5);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,uVar1);
  DELETEOBJECT(0x11d0,uVar2,param_3);
  return;
}



/* ============================================================
   1010:a64a  FUN_1010_a64a
   ============================================================ */

void __stdcall16far FUN_1010_a64a(undefined2 *param_1,undefined2 param_2)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  undefined2 uVar6;
  int iVar7;
  int extraout_DX;
  undefined2 *puVar8;
  undefined2 *puVar9;
  uint uVar10;
  undefined2 *puVar11;
  undefined2 uVar12;
  undefined2 unaff_CS;
  char *pcVar13;
  undefined2 unaff_SS;
  undefined2 uVar14;
  undefined2 uVar15;
  int local_182 [148];
  char local_5a;
  char local_59;
  undefined1 local_58;
  undefined2 local_2a;
  undefined1 local_28 [20];
  undefined1 local_14 [6];
  undefined2 local_e;
  undefined4 local_c;
  int local_8;
  uint local_6;
  int local_4;
  
  uVar12 = (undefined2)((ulong)param_1 >> 0x10);
  puVar8 = (undefined2 *)param_1;
  uVar6 = param_2;
  local_e = SELECTOBJECT(unaff_CS,puVar8[8]);
  uVar14 = param_2;
  SETTEXTCOLOR(0x11d0,0xff,0);
  iVar7 = extraout_DX;
  FUN_1008_3d50(param_2,0xc,0xc,0xbdba,0x1010,uVar14,uVar6);
  iVar4 = FUN_1000_1646(0xf0);
  local_4 = iVar7;
  if (iVar7 == 0 && iVar4 == 0) {
    local_c = 0;
  }
  else {
    local_c = FUN_1010_ec36(iVar4,iVar7);
  }
  FUN_1010_edea(local_182,unaff_SS,*param_1,puVar8[1]);
  FUN_1018_0310(local_182,unaff_SS,local_c);
  if (local_182[0] == 0) {
    SELECTOBJECT(0x1018,puVar8[6]);
    SETTEXTCOLOR(0x11d0,0xc0c0,0xc0);
    FUN_1000_16aa(&local_5a);
    FUN_1008_3d50(param_2,0xc,0x30,&local_5a);
    iVar7 = 0;
    local_59 = '\0';
    do {
      if (iVar7 < 9) {
        local_5a = (char)iVar7 + '1';
      }
      else {
        local_5a = '1';
        local_59 = (char)iVar7 + '\'';
        local_58 = 0;
      }
      FUN_1008_3d50(param_2,0xc,(iVar7 + 3) * 0x18,&local_5a);
      iVar7 = iVar7 + 1;
    } while (iVar7 < 10);
    iVar7 = 0;
    uVar3 = local_c;
    while( true ) {
      local_c._2_2_ = (undefined2)((ulong)uVar3 >> 0x10);
      local_c._0_2_ = (int)uVar3;
      iVar4 = (int)local_c;
      pcVar13 = (char *)s_Lucky_Chip_1080_1004 + 4;
      if (9 < iVar7) break;
      local_c._0_2_ = iVar7 * 0x18 + (int)local_c;
      if ((*(int *)((int)local_c + 0xe) != -1) || (*(int *)((int)local_c + 0x10) != -1)) {
        uVar14 = param_2;
        local_c = uVar3;
        SETTEXTCOLOR((char *)s_Lucky_Chip_1080_1004 + 4,0,0xff);
        FUN_1008_3d50(param_2,0x3c,(iVar7 + 3) * 0x18,iVar7 * 0x18 + iVar4,local_c._2_2_,uVar14);
        FUN_1000_1924(&local_5a);
        FUN_1000_1e96(local_28);
        local_8 = iVar7;
        local_6 = FUN_1000_1710(&local_5a);
        local_6 = local_6 % 3;
        if (local_6 == 0) {
          local_6 = 3;
        }
        local_4 = 0;
        iVar4 = 0;
        uVar10 = local_6;
        do {
          FUN_1000_172a(local_28);
          iVar7 = FUN_1000_1710(&local_5a);
          if ((int)(iVar4 + uVar10) < iVar7) {
            FUN_1000_1656(local_28);
          }
          iVar4 = iVar4 + uVar10;
          uVar10 = 3;
          iVar5 = FUN_1000_1710(&local_5a);
          iVar7 = local_8;
        } while (iVar4 < iVar5);
        SETTEXTCOLOR(0x1000,0xffff,0);
        FUN_1008_3d50(param_2,0xfc,(iVar7 + 3) * 0x18,local_28);
        FUN_1000_16aa(local_14);
        SETTEXTCOLOR(0x1000,0xff,0);
        FUN_1008_3d50(param_2,0x198,(iVar7 + 3) * 0x18,local_14);
        uVar3 = local_c;
      }
      iVar7 = iVar7 + 1;
    }
  }
  else {
    puVar11 = &local_2a;
    puVar9 = (undefined2 *)0xbdf4;
    for (iVar7 = 0xe; iVar7 != 0; iVar7 = iVar7 + -1) {
      puVar2 = puVar11;
      puVar11 = puVar11 + 1;
      puVar1 = puVar9;
      puVar9 = puVar9 + 1;
      *puVar2 = *puVar1;
    }
    SELECTOBJECT(0x1018,puVar8[7]);
    SETTEXTCOLOR(0x11d0,0,0);
    uVar15 = 0xc;
    uVar12 = 0x30;
    puVar8 = &local_2a;
    uVar14 = unaff_SS;
    uVar6 = FUN_1000_1710(puVar8);
    pcVar13 = (char *)0x11d0;
    TEXTOUT(0x1000,uVar6,puVar8,uVar14,uVar12,uVar15);
    uVar3 = local_c;
  }
  local_c = uVar3;
  SELECTOBJECT(pcVar13,local_e);
  FUN_1000_1634(local_c,param_2);
  FUN_1010_ef2e(local_182,unaff_SS);
  return;
}



/* ============================================================
   1010:a966  FUN_1010_a966
   ============================================================ */

void __stdcall16far FUN_1010_a966(undefined4 param_1,undefined2 param_2)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  int iVar3;
  int iVar4;
  undefined2 *puVar5;
  undefined2 *puVar6;
  undefined2 uVar7;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar8;
  undefined2 uVar9;
  undefined2 local_22e [277];
  undefined2 local_4;
  
  uVar7 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  uVar9 = param_2;
  local_4 = SELECTOBJECT(unaff_CS,*(undefined2 *)(iVar4 + 0x10));
  uVar8 = param_2;
  SETTEXTCOLOR(0x11d0,0xc0c0,0xc0);
  FUN_1008_3d50(param_2,0xc,0xc,0xbe10,0x1010,uVar8,uVar9);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,local_4);
  puVar6 = local_22e;
  puVar5 = (undefined2 *)0xbe22;
  for (iVar3 = 0x115; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar2 = puVar6;
    puVar6 = puVar6 + 1;
    puVar1 = puVar5;
    puVar5 = puVar5 + 1;
    *puVar2 = *puVar1;
  }
  FUN_1010_ae4a(iVar4,uVar7,0x80,0,0x168,0x1b0,0x30,0xc,local_22e,unaff_SS,
                *(undefined2 *)(iVar4 + 0xe),param_2);
  return;
}



/* ============================================================
   1010:a9ee  FUN_1010_a9ee
   ============================================================ */

void __stdcall16far FUN_1010_a9ee(undefined4 param_1,undefined2 param_2)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  int iVar3;
  int iVar4;
  undefined2 *puVar5;
  undefined2 *puVar6;
  undefined2 uVar7;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar8;
  undefined2 uVar9;
  undefined2 local_25a [299];
  undefined2 local_4;
  
  uVar7 = (undefined2)((ulong)param_1 >> 0x10);
  iVar4 = (int)param_1;
  uVar9 = param_2;
  local_4 = SELECTOBJECT(unaff_CS,*(undefined2 *)(iVar4 + 0x10));
  uVar8 = param_2;
  SETTEXTCOLOR(0x11d0,0xc0c0,0xc0);
  FUN_1008_3d50(param_2,0xc,0xc,0xc04c,0x1010,uVar8,uVar9);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,local_4);
  puVar6 = local_25a;
  puVar5 = (undefined2 *)0xc052;
  for (iVar3 = 299; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar2 = puVar6;
    puVar6 = puVar6 + 1;
    puVar1 = puVar5;
    puVar5 = puVar5 + 1;
    *puVar2 = *puVar1;
  }
  FUN_1010_ae4a(iVar4,uVar7,0x80,0,0x168,0x1b0,0x30,0xc,local_25a,unaff_SS,
                *(undefined2 *)(iVar4 + 0xe),param_2);
  return;
}



/* ============================================================
   1010:aa76  FUN_1010_aa76
   ============================================================ */

void __stdcall16far FUN_1010_aa76(int param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 unaff_CS;
  undefined2 uVar3;
  
  uVar3 = param_3;
  uVar1 = SELECTOBJECT(unaff_CS,*(undefined2 *)(param_1 + 0x10));
  uVar2 = param_3;
  SETTEXTCOLOR(0x11d0,0xff,0);
  FUN_1008_3d50(param_3,0xc,0xc,0xc2a8,0x1010,uVar2,uVar3);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,uVar1);
  uVar2 = CREATEPEN(0x11d0,0x8080,0x80,1);
  SELECTOBJECT(0x11d0,uVar2);
  uVar1 = param_3;
  MOVETO(0x11d0,0x36,0x1b0);
  uVar3 = param_3;
  LINETO(0x11d0,0x13e,0x1b0);
  uVar2 = param_3;
  LINETO(0x11d0,0x13e,0xc);
  uVar2 = GETSTOCKOBJECT(0x11d0,6,param_3,uVar2,uVar3,uVar1);
  uVar2 = SELECTOBJECT(0x11d0,uVar2);
  DELETEOBJECT(0x11d0,uVar2);
  LINETO(0x11d0,0x36,0xc);
  LINETO(0x11d0,0x36,0x1b0);
  FUN_1010_ae4a(param_1,param_2,0x80,0,0x168,600,0x42,0x18,0xc2b6,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1010_ae4a(param_1,param_2,0,0,0x168,0x1b0,0x126,0x18,0xc438,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1010_ae4a(param_1,param_2,0xff,0,0x168,0x1b0,0x126,0x18,0xc456,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1010_aba0(param_1,param_2,param_3);
  return;
}



/* ============================================================
   1010:aba0  FUN_1010_aba0
   ============================================================ */

void __stdcall16far FUN_1010_aba0(undefined2 *param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  undefined2 unaff_SS;
  undefined1 local_8 [6];
  
  FUN_1008_3b6c(local_8,unaff_SS,param_3);
  if (param_1[10] == 1) {
    uVar1 = 0x1b1;
  }
  else {
    uVar1 = 0x1b0;
  }
  FUN_1008_3c2e(local_8,unaff_SS,0x120,0x72,uVar1);
  uVar1 = FUN_1008_2d50(*param_1,param_1[1]);
  FUN_1008_3c2e(local_8,unaff_SS,(param_1[10] == 1) + 0x126,(param_1[10] == 1) + 0x78,uVar1);
  FUN_1008_3ba6(local_8,unaff_SS);
  return;
}



/* ============================================================
   1010:ac1c  FUN_1010_ac1c
   ============================================================ */

undefined2 __stdcall16far
FUN_1010_ac1c(undefined4 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 unaff_CS;
  char *pcVar6;
  undefined2 unaff_SS;
  undefined1 local_2a [4];
  int local_26;
  undefined2 local_24;
  undefined2 local_22;
  undefined1 local_18 [16];
  undefined2 local_8;
  undefined2 local_6;
  undefined2 local_4;
  
  if (DAT_1080_1bf0 == 0xb) {
    local_8 = param_3;
    local_6 = param_4;
    uVar5 = param_2;
    iVar1 = PTINRECT(unaff_CS,param_3,param_4,(undefined2 *)((int)param_1 + 0x16));
    if (iVar1 != 0) {
      *(undefined2 *)(param_1 + 5) = 1;
      uVar3 = *(undefined2 *)((int)param_1 + 6);
      uVar2 = GETDLGITEM(0x11d0,7);
      uVar3 = GETDC(0x11d0,uVar2,uVar3,uVar5);
      FUN_1010_aba0(param_1,param_2,uVar3);
      uVar5 = *(undefined2 *)((int)param_1 + 6);
      uVar2 = GETDLGITEM(0x1010,7);
      RELEASEDC(0x11d0,uVar3);
      uVar3 = *(undefined2 *)((int)param_1 + 6);
      SETCAPTURE(0x11d0,uVar3,uVar2,uVar5);
      do {
        pcVar6 = (char *)0x11d0;
        iVar1 = PEEKMESSAGE(0x11d0,1,0x113,0x113,0,local_2a,unaff_SS);
        if ((iVar1 != 0) && (local_26 == 0xb)) {
          uVar5 = (undefined2)((ulong)*param_1 >> 0x10);
          iVar1 = (int)*param_1;
          pcVar6 = (char *)s_Lucky_Chip_1080_1004 + 4;
          FUN_1008_5f92(*(undefined2 *)(iVar1 + 0xc),*(undefined2 *)(iVar1 + 0xe),0xb);
        }
        iVar1 = PEEKMESSAGE(pcVar6,1,0x200,0x200,0,local_2a,unaff_SS);
        if (iVar1 != 0) {
          local_8 = local_24;
          local_6 = local_22;
          if ((*(int *)(param_1 + 5) == 1) &&
             (uVar5 = param_2,
             iVar1 = PTINRECT(0x11d0,local_24,local_22,(undefined2 *)((int)param_1 + 0x16)),
             iVar1 == 0)) {
            *(undefined2 *)(param_1 + 5) = 0;
          }
          else {
            if ((*(int *)(param_1 + 5) != 0) ||
               (uVar5 = param_2,
               iVar1 = PTINRECT(0x11d0,local_8,local_6,(undefined2 *)((int)param_1 + 0x16)),
               iVar1 == 0)) goto LAB_1010_ad73;
            *(undefined2 *)(param_1 + 5) = 1;
          }
          uVar2 = *(undefined2 *)((int)param_1 + 6);
          uVar4 = GETDLGITEM(0x11d0,7);
          uVar5 = GETDC(0x11d0,uVar4,uVar2,uVar5);
          FUN_1010_aba0(param_1,param_2,uVar5);
          GETDLGITEM(0x1010,7);
          RELEASEDC(0x11d0,uVar5);
        }
LAB_1010_ad73:
        iVar1 = PEEKMESSAGE(0x11d0,1,0x202,0x202,0,local_2a,unaff_SS);
        if (iVar1 != 0) {
          iVar1 = *(int *)(param_1 + 5);
          *(undefined2 *)(param_1 + 5) = 0;
          uVar5 = *(undefined2 *)((int)param_1 + 6);
          uVar3 = GETDLGITEM(0x11d0,7);
          local_4 = GETDC(0x11d0,uVar3,uVar5);
          FUN_1010_aba0(param_1,param_2,local_4);
          uVar5 = *(undefined2 *)((int)param_1 + 6);
          uVar3 = GETDLGITEM(0x1010,7);
          RELEASEDC(0x11d0,local_4);
          RELEASECAPTURE(0x11d0,uVar3,uVar5);
          if (iVar1 == 1) {
            FUN_1010_dce2(local_18,unaff_SS,*(undefined2 *)(param_1 + 2),
                          *(undefined2 *)((int)param_1 + 6),param_1,param_2,*(undefined2 *)param_1,
                          *(undefined2 *)((int)param_1 + 2));
            FUN_1010_dd30(local_18,unaff_SS);
            FUN_1010_dd2c(local_18,unaff_SS);
          }
          return 0;
        }
        iVar1 = PEEKMESSAGE(0x11d0,0,0xf,0xf,0,local_2a,unaff_SS);
        if (iVar1 != 0) {
          GETMESSAGE(0x11d0,0xf,0xf,0,local_2a,unaff_SS,uVar3);
          uVar3 = unaff_SS;
          DISPATCHMESSAGE(0x11d0,local_2a);
        }
      } while( true );
    }
  }
  return 1;
}



/* ============================================================
   1010:ae4a  FUN_1010_ae4a
   ============================================================ */

void __stdcall16far
FUN_1010_ae4a(undefined4 param_1,undefined2 param_2,undefined2 param_3,int param_4,int param_5,
             int param_6,int param_7,undefined2 param_8,undefined2 param_9,int param_10,
             undefined2 param_11)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 uVar5;
  int local_3a [4];
  int local_32;
  int local_2e;
  undefined2 local_1a;
  undefined2 local_18;
  int local_16;
  int local_14;
  int local_12;
  int local_10;
  undefined2 local_e;
  undefined2 uStack_c;
  int local_a;
  undefined4 local_8;
  int local_4;
  
  _local_e = CONCAT22(param_9,param_8);
  local_18 = SELECTOBJECT(unaff_CS,param_10);
  local_8 = SETTEXTCOLOR(0x11d0,param_2,param_3);
  uVar5 = param_11;
  local_1a = SETBKMODE(0x11d0,1);
  GETTEXTMETRICS(0x11d0,local_3a,unaff_SS);
  local_16 = local_2e;
  local_10 = local_32 + local_3a[0];
  if (*(int *)((int)param_1 + 0xe) == param_10) {
    local_16 = 8;
    local_10 = 0x10;
  }
  local_12 = param_6;
  local_14 = param_7;
  local_a = 0;
  iVar3 = local_a;
  while( true ) {
    iVar1 = FUN_1000_1710(local_e,uStack_c,param_11,uVar5);
    if ((iVar1 <= iVar3) || (param_4 - local_10 < local_12)) break;
    uVar4 = (undefined2)((ulong)_local_e >> 0x10);
    iVar1 = (int)_local_e;
    if (*(char *)(iVar1 + iVar3) == '\n') {
      local_14 = param_7;
      local_12 = local_12 + local_10;
      iVar3 = iVar3 + 1;
    }
    else {
      iVar2 = 0;
      local_4 = 0;
      while (((*(char *)(iVar1 + iVar2 + iVar3) != ' ' && (*(char *)(iVar1 + iVar2 + iVar3) != '\n')
              ) && (*(char *)(iVar1 + iVar2 + iVar3) != '\0'))) {
        local_4 = local_4 + 1;
        iVar2 = iVar2 + 1;
      }
      local_a = iVar3;
      if (local_4 * local_16 + local_14 < param_5) {
        if (local_14 == param_7) {
          for (; local_a = iVar3, *(char *)(iVar1 + iVar3) == ' '; iVar3 = iVar3 + 1) {
          }
        }
        TEXTOUT(0x1000,1,iVar1 + local_a,uVar4,local_12,local_14);
        local_14 = local_14 + local_16;
        if (param_5 - local_16 < local_14) {
          local_14 = param_7;
          local_12 = local_12 + local_10;
        }
        iVar3 = local_a + 1;
        local_a = iVar3;
      }
      else {
        local_14 = param_7;
        local_12 = local_12 + local_10;
      }
    }
  }
  SETTEXTCOLOR(0x1000,local_8);
  SETBKMODE(0x11d0,local_1a);
  SELECTOBJECT(0x11d0,local_18);
  return;
}



/* ============================================================
   1010:afd8  FUN_1010_afd8
   ============================================================ */

void __stdcall16far FUN_1010_afd8(undefined4 *param_1,undefined2 param_2)

{
  undefined2 uVar1;
  undefined2 uVar2;
  int iVar3;
  undefined2 uVar4;
  int iVar5;
  char *unaff_CS;
  char *pcVar6;
  undefined2 unaff_SS;
  undefined4 local_16;
  undefined1 local_12 [6];
  undefined2 local_c;
  undefined2 local_a;
  int local_8;
  int local_6;
  undefined2 local_4;
  
  if (DAT_1080_1bf0 == 2) {
    uVar2 = *(undefined2 *)((int)param_1 + 6);
    uVar1 = GETDLGITEM(unaff_CS,7);
    local_4 = GETDC(0x11d0,uVar1,uVar2);
    FUN_1008_3b6c(&local_a,unaff_SS,local_4);
    uVar2 = FUN_1008_2d50(*(undefined2 *)param_1,*(undefined2 *)((int)param_1 + 2));
    FUN_1008_3c2e(&local_a,unaff_SS,0x5a,0xc0,uVar2);
    RELEASEDC((char *)s_Lucky_Chip_1080_1004 + 4,local_4);
    unaff_CS = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_3ba6(&local_a,unaff_SS);
  }
  if (DAT_1080_1bf0 == 6) {
    uVar2 = *(undefined2 *)((int)param_1 + 6);
    uVar1 = GETDLGITEM(unaff_CS,7);
    local_4 = GETDC(0x11d0,uVar1,uVar2);
    FUN_1008_3b6c(&local_a,unaff_SS,local_4);
    uVar2 = FUN_1008_2d50(*(undefined2 *)param_1,*(undefined2 *)((int)param_1 + 2));
    FUN_1008_3c2e(&local_a,unaff_SS,0x65,0x162,uVar2);
    RELEASEDC((char *)s_Lucky_Chip_1080_1004 + 4,local_4);
    unaff_CS = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_3ba6(&local_a,unaff_SS);
  }
  if (DAT_1080_1bf0 != 4) goto LAB_1010_b2cb;
  uVar2 = *(undefined2 *)((int)param_1 + 6);
  local_c = GETDLGITEM(unaff_CS,7);
  local_a = GETDC(0x11d0,local_c,uVar2);
  FUN_1008_3b6c(local_12,unaff_SS,local_a);
  uVar2 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar5 = (int)*param_1;
  uVar2 = FUN_1008_ced2(*(undefined2 *)(iVar5 + 0xdc),*(undefined2 *)(iVar5 + 0xde));
  FUN_1008_3bd0(local_12,unaff_SS,5,0x10,uVar2);
  uVar2 = (undefined2)((ulong)*param_1 >> 0x10);
  iVar5 = (int)*param_1;
  uVar2 = FUN_1008_cf26(*(undefined2 *)(iVar5 + 0xdc),*(undefined2 *)(iVar5 + 0xde));
  FUN_1008_3bd0(local_12,unaff_SS,4,0x22,uVar2);
  if ((int)DAT_1080_14ce % 8 < 4) {
    local_6 = (int)DAT_1080_14ce % 4 + 0x2c;
  }
  else {
    local_6 = 0x2b;
  }
  FUN_1008_3bd0(local_12,unaff_SS,0xc,5,local_6);
  if ((int)DAT_1080_14ce % 8 < 4) {
    local_8 = (int)DAT_1080_14ce % 4 + 0x42;
  }
  else {
    local_8 = 0x41;
  }
  FUN_1008_3bd0(local_12,unaff_SS,0xc,7,local_8);
  FUN_1008_3bd0(local_12,unaff_SS,0x19,10,(int)DAT_1080_14ce % 4 + 0x57);
  local_4 = 8;
  for (iVar5 = 8; iVar5 < 0xd; iVar5 = iVar5 + 4) {
    if ((int)DAT_1080_14ce % 6 == 0) {
      uVar2 = 0x46;
LAB_1010_b1e3:
      FUN_1008_3bd0(local_12,unaff_SS,0x19,iVar5,uVar2);
    }
    else {
      if (((int)DAT_1080_14ce % 6 == 4) && (iVar3 = FUN_1008_3c88(3), iVar3 == 1)) {
        uVar2 = 0x47;
        goto LAB_1010_b1e3;
      }
      if (((int)DAT_1080_14ce % 6 == 5) && (iVar3 = FUN_1008_3c88(3), iVar3 == 1)) {
        uVar2 = 0x48;
        goto LAB_1010_b1e3;
      }
    }
  }
  if ((int)DAT_1080_14ce % 5 == 1) {
    uVar2 = 0x6d;
LAB_1010_b215:
    FUN_1008_3bd0(local_12,unaff_SS,0xc,0x11,uVar2);
  }
  else if ((int)DAT_1080_14ce % 5 == 2) {
    uVar2 = 0x6e;
    goto LAB_1010_b215;
  }
  pcVar6 = (char *)s_Lucky_Chip_1080_1004 + 4;
  if ((DAT_1080_14ce & 1) == 0) {
    local_4 = 0;
    local_6 = 0;
    if ((int)DAT_1080_14ce % 8 == 0) {
      local_6 = 0xff;
      local_4 = 0;
    }
    else if ((int)DAT_1080_14ce % 8 == 4) {
      local_6 = 0;
      local_4 = 0xff;
    }
    uVar1 = 0;
    uVar2 = local_a;
    local_8 = CREATEPEN((char *)s_Lucky_Chip_1080_1004 + 4,local_6,local_4,1);
    uVar4 = SELECTOBJECT(0x11d0,local_8);
    uVar2 = GETSTOCKOBJECT(0x11d0,5,local_a,uVar1,uVar2);
    SELECTOBJECT(0x11d0,uVar2);
    uVar1 = local_a;
    RECTANGLE(0x11d0,0x138,0x1b0,0x108,0xe4);
    uVar2 = local_a;
    SELECTOBJECT(0x11d0,uVar4);
    pcVar6 = (char *)0x11d0;
    DELETEOBJECT(0x11d0,local_8,uVar2,uVar1);
  }
  RELEASEDC(pcVar6,local_a);
  unaff_CS = (char *)s_Lucky_Chip_1080_1004 + 4;
  FUN_1008_3ba6(local_12,unaff_SS);
LAB_1010_b2cb:
  if (DAT_1080_1bf0 == 5) {
    uVar2 = *(undefined2 *)((int)param_1 + 6);
    uVar1 = GETDLGITEM(unaff_CS,7);
    local_4 = GETDC(0x11d0,uVar1,uVar2);
    FUN_1008_3b6c(&local_a,unaff_SS,local_4);
    FUN_1008_3c2e(&local_a,unaff_SS,0x130,0x18,(int)DAT_1080_14ce % 4 + 0x1d);
    FUN_1008_3c2e(&local_a,unaff_SS,0xe8,0x18,(int)DAT_1080_14ce % 4 + 0x21);
    RELEASEDC((char *)s_Lucky_Chip_1080_1004 + 4,local_4);
    unaff_CS = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_3ba6(&local_a,unaff_SS);
  }
  if (DAT_1080_1bf0 == 0xb) {
    uVar2 = *(undefined2 *)((int)param_1 + 6);
    uVar1 = GETDLGITEM(unaff_CS,7);
    local_4 = GETDC(0x11d0,uVar1,uVar2);
    FUN_1008_3b6c(&local_a,unaff_SS,local_4);
    FUN_1008_3c2e(&local_a,unaff_SS,0x92,0x14,(int)DAT_1080_14ce % 4 + 0x1d);
    FUN_1008_3c2e(&local_a,unaff_SS,0xb6,0x14,(int)DAT_1080_14ce % 4 + 0x1d);
    uVar2 = FUN_1008_2d50(*(undefined2 *)param_1,*(undefined2 *)((int)param_1 + 2));
    FUN_1008_3c2e(&local_a,unaff_SS,(*(int *)(param_1 + 5) == 1) + 0x126,
                  (*(int *)(param_1 + 5) == 1) + 0x78,uVar2);
    RELEASEDC((char *)s_Lucky_Chip_1080_1004 + 4,local_4);
    unaff_CS = (char *)s_Lucky_Chip_1080_1004 + 4;
    FUN_1008_3ba6(&local_a,unaff_SS);
  }
  if (DAT_1080_1bf0 == 0xc) {
    uVar2 = *(undefined2 *)((int)param_1 + 6);
    local_16 = (undefined4 *)CONCAT22(param_2,param_1);
    uVar1 = GETDLGITEM(unaff_CS,7);
    local_4 = GETDC(0x11d0,uVar1,uVar2);
    FUN_1008_3b6c(&local_a,unaff_SS,local_4);
    FUN_1008_3c2e(&local_a,unaff_SS,0x3f,10,(int)DAT_1080_14ce % 4 + 0x1d);
    FUN_1008_3c2e(&local_a,unaff_SS,0x4e,10,(int)DAT_1080_14ce % 4 + 0x1d);
    FUN_1008_3c2e(&local_a,unaff_SS,0x5d,10,(int)DAT_1080_14ce % 4 + 0x1d);
    uVar2 = FUN_1008_2d50(*(undefined2 *)local_16,*(undefined2 *)((int)param_1 + 2));
    FUN_1008_3c2e(&local_a,unaff_SS,0x120,0x168,uVar2);
    RELEASEDC((char *)s_Lucky_Chip_1080_1004 + 4,local_4);
    FUN_1008_3ba6(&local_a,unaff_SS);
  }
  return;
}



/* ============================================================
   1010:b4ae  FUN_1010_b4ae
   ============================================================ */

void __stdcall16far FUN_1010_b4ae(int param_1,undefined2 param_2,undefined2 param_3)

{
  int iVar1;
  int in_DX;
  long lVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  
  uVar5 = 0x13a;
  uVar4 = 0x180;
  uVar3 = 0xfc;
  iVar1 = FUN_1000_1646(6,0xfc,0x180,0x13a);
  if (in_DX == 0 && iVar1 == 0) {
    lVar2 = 0;
  }
  else {
    lVar2 = FUN_1008_3b6c(iVar1,in_DX,param_3);
  }
  FUN_1008_3c2e(lVar2,uVar3,uVar4,uVar5);
  FUN_1010_ae4a(param_1,param_2,0,0xff,0x168,0x1b0,0xc,0xc,0xc4d6,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1010_ae4a(param_1,param_2,0,0,0x168,0x1b0,0x3c,0xc,0xc510,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  uVar4 = param_3;
  uVar5 = SELECTOBJECT(0x1010,*(undefined2 *)(param_1 + 0x10));
  uVar3 = param_3;
  SETTEXTCOLOR(0x11d0,0xc0c0,0);
  FUN_1008_3d50(param_3,0xc,0x84,0xc59c,0x1010,uVar3,uVar4);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,uVar5);
  FUN_1010_ae4a(param_1,param_2,0xff,0,0x168,0x1b0,0xcc,0xc,0xc5b0,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1010_ae4a(param_1,param_2,0x80,0,0x168,0x1b0,0xcc,0xc,0xc5ce,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1010_ae4a(param_1,param_2,0xff,0,0x168,0x1b0,0x108,0xc,0xc656,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  FUN_1010_ae4a(param_1,param_2,0x80,0,0x168,0x1b0,0x108,0xc,0xc668,0x1010,
                *(undefined2 *)(param_1 + 0xe),param_3);
  if (lVar2 != 0) {
    FUN_1008_3ba6(lVar2);
    FUN_1000_1634(lVar2);
  }
  return;
}



/* ============================================================
   1010:b638  FUN_1010_b638
   ============================================================ */

void __stdcall16far FUN_1010_b638(undefined4 param_1,undefined2 param_2)

{
  int iVar1;
  undefined2 uVar2;
  int in_DX;
  int iVar3;
  undefined2 uVar4;
  char *pcVar5;
  long lVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  int local_4;
  
  pcVar5 = (char *)0x1000;
  iVar1 = FUN_1000_1646(6);
  if (in_DX == 0 && iVar1 == 0) {
    lVar6 = 0;
  }
  else {
    pcVar5 = (char *)s_Lucky_Chip_1080_1004 + 4;
    lVar6 = FUN_1008_3b6c(iVar1,in_DX,param_2);
  }
  uVar4 = (undefined2)((ulong)param_1 >> 0x10);
  iVar1 = (int)param_1;
  uVar8 = param_2;
  uVar2 = SELECTOBJECT(pcVar5,*(undefined2 *)(iVar1 + 0x10));
  uVar7 = param_2;
  SETTEXTCOLOR(0x11d0,0xc0c0,0);
  FUN_1008_3d50(param_2,0xc,0xc,0xc6b8,0x1010,uVar7,uVar8);
  SELECTOBJECT((char *)s_Lucky_Chip_1080_1004 + 4,uVar2);
  for (local_4 = 1; local_4 < 4; local_4 = local_4 + 1) {
    iVar3 = 4;
    do {
      FUN_1008_3c2e(lVar6,iVar3 * 0xc + 4,local_4 * 0xc,0);
      iVar3 = iVar3 + 1;
    } while (iVar3 < 0x1a);
  }
  FUN_1010_ae4a(iVar1,uVar4,0xff,0,0x168,0x1b0,0x30,0x48,0x691e,(char *)s_Lucky_Chip_1080_1004 + 4,
                *(undefined2 *)(iVar1 + 0xe),param_2);
  FUN_1010_ae4a(iVar1,uVar4,0x80,0,0x168,0x1b0,0x3c,0x48,0xc6c6,0x1010,*(undefined2 *)(iVar1 + 0xe),
                param_2);
  FUN_1008_3c2e(lVar6,0x34,0x18,0x12);
  FUN_1010_ae4a(iVar1,uVar4,0xff,0,0x168,0x1b0,0x54,0x48,0x6932,(char *)s_Lucky_Chip_1080_1004 + 4,
                *(undefined2 *)(iVar1 + 0xe),param_2);
  FUN_1010_ae4a(iVar1,uVar4,0x80,0,0x168,0x1b0,0x60,0x48,0xc6e6,0x1010,*(undefined2 *)(iVar1 + 0xe),
                param_2);
  FUN_1008_3c2e(lVar6,0x58,0x18,0x13);
  FUN_1010_ae4a(iVar1,uVar4,0xff,0,0x168,0x1b0,0x78,0x48,0x6938,(char *)s_Lucky_Chip_1080_1004 + 4,
                *(undefined2 *)(iVar1 + 0xe),param_2);
  FUN_1010_ae4a(iVar1,uVar4,0x80,0,0x168,0x1b0,0x84,0x48,0xc6fe,0x1010,*(undefined2 *)(iVar1 + 0xe),
                param_2);
  FUN_1008_3c2e(lVar6,0x7c,0x18,0x16);
  FUN_1010_ae4a(iVar1,uVar4,0xff,0,0x168,0x1b0,0x9c,0x48,0xd248,(char *)s_Lucky_Chip_1080_1004 + 4,
                *(undefined2 *)(iVar1 + 0xe),param_2);
  FUN_1010_ae4a(iVar1,uVar4,0x80,0,0x168,0x1b0,0xa8,0x48,0xc728,0x1010,*(undefined2 *)(iVar1 + 0xe),
                param_2);
  FUN_1008_3c2e(lVar6,0xa0,0x18,0x14);
  FUN_1010_ae4a(iVar1,uVar4,0xff,0,0x168,0x1b0,0xc0,0x48,0x69c0,(char *)s_Lucky_Chip_1080_1004 + 4,
                *(undefined2 *)(iVar1 + 0xe),param_2);
  FUN_1010_ae4a(iVar1,uVar4,0x80,0,0x168,0x1b0,0xcc,0x48,0xc74c,0x1010,*(undefined2 *)(iVar1 + 0xe),
                param_2);
  FUN_1008_3c2e(lVar6,0xc4,0x18,0x154);
  FUN_1010_ae4a(iVar1,uVar4,0xff,0,0x168,0x1b0,0xe4,0x48,0x6972,(char *)s_Lucky_Chip_1080_1004 + 4,
                *(undefined2 *)(iVar1 + 0xe),param_2);
  FUN_1010_ae4a(iVar1,uVar4,0x80,0,0x168,0x1b0,0xf0,0x48,0xc774,0x1010,*(undefined2 *)(iVar1 + 0xe),
                param_2);
  FUN_1008_3c2e(lVar6,0xe8,0x18,0x21);
  FUN_1010_ae4a(iVar1,uVar4,0xff,0,0x168,0x1b0,0x108,0x48,0xd27e,(char *)s_Lucky_Chip_1080_1004 + 4,
                *(undefined2 *)(iVar1 + 0xe),param_2);
  FUN_1010_ae4a(iVar1,uVar4,0x80,0,0x168,0x1b0,0x114,0x48,0xc794,0x1010,*(undefined2 *)(iVar1 + 0xe)
                ,param_2);
  FUN_1008_3c2e(lVar6,0x10c,0x18,0x13);
  FUN_1010_ae4a(iVar1,uVar4,0xff,0,0x168,0x1b0,300,0x48,0xc7ba,0x1010,*(undefined2 *)(iVar1 + 0xe),
                param_2);
  FUN_1010_ae4a(iVar1,uVar4,0x80,0,0x168,0x1b0,0x138,0x48,0xc7d0,0x1010,*(undefined2 *)(iVar1 + 0xe)
                ,param_2);
  FUN_1008_3c2e(lVar6,0x130,0x18,0x1d);
  if (lVar6 != 0) {
    FUN_1008_3ba6(lVar6);
    FUN_1000_1634(lVar6);
  }
  return;
}



/* ============================================================
   1010:c80e  FUN_1010_c80e
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_c80e(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  undefined4 uVar1;
  int iVar2;
  int local_4;
  
  FUN_1008_7560(param_1,param_2,param_3,param_4,param_5,param_6);
  *param_1 = 0xcb4c;
  param_1[1] = 0x1010;
  FUN_1008_9504(param_1,param_2,3,0x14,0x14);
  FUN_1008_ce88(param_1,param_2,0,0x1b,0x20,8,8);
  FUN_1008_ce88(param_1,param_2,0,8,0x15,6,0x13);
  uVar1 = *(undefined4 *)((int)*(undefined4 *)(param_1 + 4) + 0x50);
  *(undefined2 *)((int)uVar1 + 0xe) = 0x23;
  local_4 = 0;
  do {
    for (iVar2 = 0; iVar2 < 0xb; iVar2 = iVar2 + 5) {
      FUN_1008_c452(param_1,param_2,iVar2 + 0xc,local_4 + 0xc);
    }
    local_4 = local_4 + 5;
  } while (local_4 < 0x10);
  param_1[0x25] = 6;
  param_1[0x27] = 0x19;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:c8cc  FUN_1010_c8cc
   ============================================================ */

void __stdcall16far FUN_1010_c8cc(undefined4 param_1,int param_2)

{
  undefined2 uVar1;
  int iVar2;
  int in_DX;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 unaff_SS;
  undefined4 uVar5;
  undefined2 *puVar6;
  undefined1 local_e [4];
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_4;
  
  if (param_2 < 0x28) {
    if (*(int *)((int)param_1 + 0x16) == 0 && *(int *)((int)param_1 + 0x14) == 0) {
      iVar2 = FUN_1000_1646(0x56);
      if (in_DX == 0 && iVar2 == 0) {
        *(undefined2 *)((int)param_1 + 0x16) = 0;
        *(undefined2 *)((int)param_1 + 0x14) = 0;
      }
      else {
        uVar5 = FUN_1010_cb9c(iVar2,in_DX,0,0,*(int *)((int)param_1 + 0x3c) + 1,
                              *(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6));
        *(undefined2 *)((int)param_1 + 0x14) = (int)uVar5;
        *(undefined2 *)((int)param_1 + 0x16) = (int)((ulong)uVar5 >> 0x10);
      }
      uVar3 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 0x14) >> 0x10);
      iVar2 = (int)*(undefined4 *)((int)param_1 + 0x14);
      *(int *)(iVar2 + 0xe) = (int)param_1;
      *(undefined2 *)(iVar2 + 0x10) = param_1._2_2_;
    }
    uVar3 = *(undefined2 *)((int)param_1 + 0x14);
    uVar1 = *(undefined2 *)((int)param_1 + 0x16);
    local_4 = uVar1;
    puVar6 = (undefined2 *)FUN_1008_b946(uVar3,uVar1,0,local_e,unaff_SS);
    local_a = *puVar6;
    local_8 = ((undefined2 *)puVar6)[1];
    *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xea) = local_a;
    *(undefined2 *)((int)*(undefined4 *)((int)param_1 + 4) + 0xec) = local_8;
    uVar4 = (undefined2)((ulong)*(undefined4 *)((int)param_1 + 4) >> 0x10);
    iVar2 = (int)*(undefined4 *)((int)param_1 + 4);
    *(undefined2 *)(iVar2 + 0xdc) = uVar3;
    *(undefined2 *)(iVar2 + 0xde) = uVar1;
    FUN_1008_a5fa();
    DAT_1080_1070 = 1;
    FUN_1008_a652(uVar3,uVar1);
    REDRAWWINDOW((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,0);
  }
  else {
    FUN_1008_2d02(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0,0xdfae,
                  (char *)s_Lucky_Chip_1080_1004 + 4);
  }
  return;
}



/* ============================================================
   1010:c9f8  FUN_1010_c9f8
   ============================================================ */

void __stdcall16far
FUN_1010_c9f8(undefined4 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  bool bVar6;
  int local_a;
  int local_8;
  
  if (*(int *)((int)param_1 + 0x42) == 0) {
    bVar6 = true;
    iVar4 = 0;
    do {
      for (iVar5 = 0; iVar5 < 0xb; iVar5 = iVar5 + 5) {
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((int)param_1 + 8) + iVar4 * 4 + 0x30);
        if (*(int *)((int)uVar1 + iVar5 * 2 + 0x18) != 0x7f) {
          bVar6 = false;
        }
      }
      iVar4 = iVar4 + 5;
    } while (iVar4 < 0x10);
    if (!bVar6) {
      FUN_1008_2822(*(undefined2 *)((int)param_1 + 4),*(undefined2 *)((int)param_1 + 6),0xcb26,
                    0x1010);
      *(undefined2 *)((int)param_1 + 0x4a) = 10;
      *(undefined2 *)((int)param_1 + 0x4c) = 4;
      *(undefined2 *)((int)param_1 + 0x4e) = 4;
      local_8 = 200;
      local_a = 0;
      do {
        iVar4 = 0;
        do {
          iVar5 = FUN_1008_3c88(0x24);
          iVar2 = FUN_1008_3c88(0x20);
          iVar3 = FUN_1008_ce4e((int)param_1,param_1._2_2_,iVar2 + 2,iVar5 + 2,iVar2,iVar5);
          if (iVar3 == 0) {
            iVar4 = FUN_1008_cf50((int)param_1,param_1._2_2_,0,0x18,iVar2,iVar5);
            iVar5 = local_8 + -1;
            bVar6 = local_8 < 0;
            local_8 = iVar5;
            if (bVar6) break;
          }
        } while (iVar4 == 0);
      } while ((-1 < local_8) && (local_a = local_a + 1, local_a < 0xc));
      *(undefined2 *)((int)param_1 + 0x42) = 1;
    }
  }
  FUN_1008_ba30((int)param_1,param_1._2_2_,param_2,param_3,param_4);
  return;
}



/* ============================================================
   1010:cb9c  FUN_1010_cb9c
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_cb9c(undefined2 *param_1,int param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  int iVar7;
  int iVar8;
  
  FUN_1008_7560((undefined2 *)param_1,param_1._2_2_,param_3,param_4,param_5,param_6);
  *param_1 = 0xd2ea;
  ((undefined2 *)param_1)[1] = 0x1010;
  iVar7 = *(int *)((int)*(undefined4 *)((undefined2 *)param_1 + 2) + 0xe4);
  iVar2 = FUN_1008_3c88(iVar7 + 3);
  for (iVar8 = 0; iVar8 < iVar2; iVar8 = iVar8 + 1) {
    uVar4 = FUN_1008_3c88(0x26);
    uVar5 = FUN_1008_3c88(0x22,uVar4);
    iVar3 = FUN_1008_3c88(3,uVar5);
    if (iVar3 < 2) {
      uVar6 = 2;
    }
    else {
      uVar6 = 7;
    }
    FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],uVar6,uVar5,uVar4);
  }
  iVar2 = 0;
  while( true ) {
    iVar8 = FUN_1008_3c88(10);
    if (iVar8 <= iVar2) break;
    iVar8 = FUN_1008_3c88(0x26);
    iVar3 = FUN_1008_3c88(0x22);
    uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar8 * 4);
    *(undefined2 *)((int)uVar1 + iVar3 * 2) = 0xaf;
    iVar2 = iVar2 + 1;
  }
  iVar2 = 0;
  do {
    if (((undefined2 *)param_1)[0x1e] + iVar7 + -1 <= iVar2) {
      iVar2 = FUN_1008_3c88(3);
      FUN_1008_9504((undefined2 *)param_1,param_1._2_2_,3,8,iVar2 + 1);
      do {
        do {
          iVar2 = FUN_1008_3c88(0x25);
          iVar8 = FUN_1008_3c88(0x21);
          uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar2 * 4);
        } while (*(int *)((int)uVar1 + iVar8 * 2) != 0);
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar2 * 4 + 8);
      } while (*(int *)((int)uVar1 + iVar8 * 2 + 4) != 0);
      uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar2 * 4);
      *(undefined2 *)((int)uVar1 + iVar8 * 2) = 0x23;
      if (param_2 == 1) {
        uVar1 = *(undefined4 *)(iVar2 * 4 + (int)*(undefined4 *)((undefined2 *)param_1 + 4) + 8);
        *(undefined2 *)((int)uVar1 + iVar8 * 2 + 4) = 0x7e;
      }
      do {
        iVar2 = FUN_1008_3c88(0x26);
        iVar8 = FUN_1008_3c88(0x22);
        uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar2 * 4);
      } while (*(int *)((int)uVar1 + iVar8 * 2) != 0);
      uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + iVar2 * 4);
      *(undefined2 *)((int)uVar1 + iVar8 * 2) = 0x27;
      iVar2 = ((undefined2 *)param_1)[0x1e];
      iVar8 = iVar2;
      if (iVar2 < 8) {
        iVar8 = 8;
      }
      if (iVar2 / 2 < iVar8) {
        iVar8 = iVar2 / 2;
      }
      ((undefined2 *)param_1)[0x25] = iVar8;
      ((undefined2 *)param_1)[0x26] = iVar7;
      iVar7 = (100 - ((undefined2 *)param_1)[0x1e]) + iVar7 * -0xf;
      if (iVar7 < 10) {
        iVar7 = 10;
      }
      ((undefined2 *)param_1)[0x27] = iVar7;
      return param_1;
    }
    iVar8 = FUN_1008_3c88(5);
    if (iVar8 < 3) {
      iVar8 = FUN_1008_3c88(5);
      if (3 < iVar8) {
        uVar4 = FUN_1008_3c88(0x24);
        uVar5 = FUN_1008_3c88(0x20,uVar4);
        iVar8 = FUN_1008_3c88(4,uVar5);
        if (iVar8 == 3) {
          uVar6 = 0x1b;
        }
        else {
LAB_1010_ccff:
          uVar6 = 0x19;
        }
        goto LAB_1010_cd02;
      }
      iVar8 = FUN_1008_3c88(5);
      if (3 < iVar8) {
        uVar4 = FUN_1008_3c88(0x24);
        uVar5 = FUN_1008_3c88(0x20,uVar4);
        iVar8 = FUN_1008_3c88(4,uVar5);
        if (iVar8 != 3) goto LAB_1010_ccff;
        uVar6 = 0x1c;
        goto LAB_1010_cd02;
      }
    }
    else {
      uVar4 = FUN_1008_3c88(0x24);
      uVar5 = FUN_1008_3c88(0x20,uVar4);
      iVar8 = FUN_1008_3c88(4,uVar5);
      if (iVar8 == 3) {
        uVar6 = 0x1a;
      }
      else {
        uVar6 = 0x18;
      }
LAB_1010_cd02:
      FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,0,uVar6,uVar5,uVar4);
    }
    iVar2 = iVar2 + 1;
  } while( true );
}



/* ============================================================
   1010:d298  FUN_1010_d298
   ============================================================ */

void __stdcall16far FUN_1010_d298(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar1 = *(undefined4 *)((int)param_1 + 4);
  uVar3 = (undefined2)((ulong)uVar1 >> 0x10);
  iVar2 = (int)uVar1;
  FUN_1008_67e6(*(undefined2 *)(iVar2 + 0xc),*(undefined2 *)(iVar2 + 0xe),0xd2c6,0x1010);
  return;
}



/* ============================================================
   1010:d33a  FUN_1010_d33a
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_d33a(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_8;
  int local_4;
  
  FUN_1008_7560((undefined2 *)param_1,param_1._2_2_,param_2,param_3,param_4,param_5);
  *param_1 = 0xd8a4;
  ((undefined2 *)param_1)[1] = 0x1010;
  FUN_1008_cafa((undefined2 *)param_1,param_1._2_2_,0xffff,0x23,0x26,0,1);
  iVar7 = 0;
  do {
    FUN_1008_c40a((undefined2 *)param_1,param_1._2_2_,0x21,iVar7,0);
    FUN_1008_c40a((undefined2 *)param_1,param_1._2_2_,0x21,iVar7,0x27);
    iVar7 = iVar7 + 1;
  } while (iVar7 < 0x24);
  FUN_1008_cafa((undefined2 *)param_1,param_1._2_2_,0xffff,0x21,0x12,0x20,4);
  FUN_1008_cafa((undefined2 *)param_1,param_1._2_2_,0xffff,0x21,0x23,0x20,0x15);
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x60);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x7e;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x5c);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x3b;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 100);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x3a;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 100);
  *(undefined2 *)((int)uVar1 + 0x40) = 0x3f;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x5c);
  *(undefined2 *)((int)uVar1 + 0x40) = 0x3f;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x3c);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x23;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x38);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x3b;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x40);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x3a;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x40);
  *(undefined2 *)((int)uVar1 + 0x40) = 0x3f;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x38);
  *(undefined2 *)((int)uVar1 + 0x40) = 0x3f;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x78);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x27;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x74);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x3b;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x7c);
  *(undefined2 *)((int)uVar1 + 0x42) = 0x3a;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x7c);
  *(undefined2 *)((int)uVar1 + 0x40) = 0x3f;
  uVar1 = *(undefined4 *)((int)*(undefined4 *)((undefined2 *)param_1 + 4) + 0x74);
  *(undefined2 *)((int)uVar1 + 0x40) = 0x3f;
  local_4 = 2;
  do {
    iVar7 = 1;
    do {
      iVar4 = iVar7;
      iVar5 = local_4;
      iVar2 = FUN_1008_3c88(0x19,iVar7,local_4);
      if (iVar2 < 2) {
        uVar3 = 0x19;
      }
      else {
        uVar3 = 0x18;
      }
      FUN_1008_c1e6((undefined2 *)param_1,param_1._2_2_,0,uVar3,iVar4,iVar5);
      iVar7 = iVar7 + 3;
    } while (iVar7 < 0x1d);
    local_4 = local_4 + 3;
  } while (local_4 < 0x24);
  iVar7 = FUN_1008_3c88(0xc);
  iVar4 = FUN_1008_3c88(10);
  iVar4 = iVar4 * 3 + -1;
  FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],0xe,iVar4,iVar7 * 3);
  local_8 = 0;
  do {
    if (0xb < local_8) {
      FUN_1008_c452((undefined2 *)param_1,param_1._2_2_,0x20,2);
      FUN_1008_c452((undefined2 *)param_1,param_1._2_2_,0x20,0x13);
      FUN_1008_c452((undefined2 *)param_1,param_1._2_2_,0x20,0x24);
      ((undefined2 *)param_1)[0x25] = 0;
      ((undefined2 *)param_1)[0x26] = 1;
      ((undefined2 *)param_1)[0x27] = 100;
      return param_1;
    }
    do {
      iVar5 = FUN_1008_3c88(0xc);
      iVar5 = iVar5 * 3;
      iVar2 = FUN_1008_3c88(10);
      iVar2 = iVar2 * 3 + -1;
      if (iVar7 * 3 != iVar5) break;
    } while (iVar4 == iVar2);
    iVar6 = FUN_1008_3c88(5,iVar2,iVar5);
    if (iVar6 == 5) {
      uVar3 = 2;
    }
    else {
      uVar3 = 1;
    }
    FUN_1008_af5a(((undefined2 *)param_1)[0x1b],((undefined2 *)param_1)[0x1c],uVar3,iVar2,iVar5);
    local_8 = local_8 + 1;
  } while( true );
}



/* ============================================================
   1010:d826  FUN_1010_d826
   ============================================================ */

void __stdcall16far FUN_1010_d826(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar1 = *(undefined4 *)((int)param_1 + 4);
  uVar3 = (undefined2)((ulong)uVar1 >> 0x10);
  iVar2 = (int)uVar1;
  FUN_1008_67e6(*(undefined2 *)(iVar2 + 0xc),*(undefined2 *)(iVar2 + 0xe),0xd88a,0x1010);
  return;
}



/* ============================================================
   1010:d8f4  FUN_1010_d8f4
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_d8f4(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6)

{
  FUN_1008_7560(param_1,param_2,param_3,param_4,param_5,param_6);
  *param_1 = 0xdc1a;
  param_1[1] = 0x1010;
  FUN_1008_b164(param_1,param_2,0,(undefined2 *)&DAT_1080_1078);
  FUN_1008_c1e6(param_1,param_2,1,0x1e,0x20,0x1f);
  FUN_1008_c1e6(param_1,param_2,1,0x1f,5,5);
  param_1[0x25] = 4;
  param_1[0x27] = 0x4b;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:dafe  FUN_1010_dafe
   ============================================================ */

undefined2 __stdcall16far FUN_1010_dafe(undefined4 param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  
  if (param_2 == 0) {
    uVar3 = *(undefined2 *)(DAT_1080_1be6 * 4 + 0x207a);
  }
  else {
    uVar3 = *(undefined2 *)(DAT_1080_1be6 * 4 + 0x207c);
    DAT_1080_1be6 = DAT_1080_1be6 + 1;
    if (*(int *)(DAT_1080_1be6 * 4 + 0x207c) == 2) {
      uVar4 = (undefined2)((ulong)param_1 >> 0x10);
      iVar1 = *(int *)((int)param_1 + 4);
      uVar4 = *(undefined2 *)((int)param_1 + 6);
      DAT_1080_2a26 = CONCAT22(uVar4,iVar1);
      FUN_1008_045e(iVar1,uVar4,*(undefined2 *)(iVar1 + 0xd8),*(undefined2 *)(iVar1 + 0xda));
      FUN_1008_169c((undefined2)DAT_1080_2a26,DAT_1080_2a26._2_2_);
      FUN_1008_a5fa();
      DAT_1080_1070 = 1;
      uVar4 = (undefined2)((ulong)DAT_1080_2a26 >> 0x10);
      FUN_1008_a652(*(undefined2 *)((int)DAT_1080_2a26 + 0xdc),
                    *(undefined2 *)((int)DAT_1080_2a26 + 0xde));
      uVar2 = *(undefined4 *)((int)DAT_1080_2a26 + 8);
      INVALIDATERECT((char *)s_Lucky_Chip_1080_1004 + 4,1,0,0,*(undefined2 *)((int)uVar2 + 4));
      DAT_1080_1be6 = 0;
    }
  }
  return uVar3;
}



/* ============================================================
   1010:dce2  FUN_1010_dce2
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_dce2(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6,undefined2 param_7,undefined2 param_8)

{
  DAT_1080_2470 = param_1;
  DAT_1080_2472 = param_2;
  *param_1 = param_7;
  param_1[1] = param_8;
  param_1[2] = param_5;
  param_1[3] = param_6;
  param_1[4] = param_4;
  param_1[6] = param_3;
  param_1[5] = 0;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:dd2c  FUN_1010_dd2c
   ============================================================ */

void __stdcall16far FUN_1010_dd2c(void)

{
  return;
}



/* ============================================================
   1010:dd30  FUN_1010_dd30
   ============================================================ */

void __stdcall16far FUN_1010_dd30(int param_1,undefined2 param_2)

{
  undefined2 unaff_CS;
  undefined4 uVar1;
  
  uVar1 = MAKEPROCINSTANCE(unaff_CS,*(undefined2 *)(param_1 + 0xc),0xdc6a);
  DIALOGBOX(0x11d0,uVar1,*(undefined2 *)(param_1 + 8),0xe870,0x1010);
  FREEPROCINSTANCE(0x11d0,(int)uVar1);
  return;
}



/* ============================================================
   1010:dd7a  FUN_1010_dd7a
   ============================================================ */

undefined2 __stdcall16far FUN_1010_dd7a(void)

{
  undefined2 unaff_CS;
  
  ENDDIALOG(unaff_CS,0);
  return 0;
}



/* ============================================================
   1010:ddde  FUN_1010_ddde
   ============================================================ */

undefined2 __stdcall16far FUN_1010_ddde(int param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined2 local_a;
  undefined2 local_8;
  
  *(undefined2 *)(param_1 + 10) = param_3;
  GETWINDOWRECT(unaff_CS,&local_a,unaff_SS);
  MOVEWINDOW(0x11d0,1,0x1d4,0x27c,local_8,local_a);
  GETDLGITEM(0x11d0,100);
  MOVEWINDOW(0x11d0,1,0x18,0xc0,0xa8,0x1a4);
  GETDLGITEM(0x11d0,0x65);
  MOVEWINDOW(0x11d0,1,0x18,0xc0,0xcc,0x1a4);
  GETDLGITEM(0x11d0,0x66);
  MOVEWINDOW(0x11d0,1,0x18,0xc0,0xf0,0x1a4);
  GETDLGITEM(0x11d0,0x67);
  MOVEWINDOW(0x11d0,1,0x18,0x78,0x114,0x1a4);
  GETDLGITEM(0x11d0,0x68);
  MOVEWINDOW(0x11d0,1,0x18,0x78,0x138,0x1a4);
  GETDLGITEM(0x11d0,300);
  MOVEWINDOW(0x11d0,1,0x1e,0xb4,0x15c,0x150);
  GETDLGITEM(0x11d0,1);
  MOVEWINDOW(0x11d0,1,0x1e,0x48,0x15c,0x210);
  return 0;
}



/* ============================================================
   1010:ec10  FUN_1010_ec10
   ============================================================ */

undefined4 __stdcall16far FUN_1010_ec10(undefined2 param_1,undefined2 param_2)

{
  FUN_1000_1e96(param_1,param_2,0,0x18);
  return CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:ec36  FUN_1010_ec36
   ============================================================ */

undefined4 __stdcall16far FUN_1010_ec36(int param_1,undefined2 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 10;
  iVar1 = param_1;
  do {
    FUN_1010_ec10(iVar1,param_2);
    iVar1 = iVar1 + 0x18;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  FUN_1000_1e96(param_1,param_2,0,0xf0);
  iVar1 = 0;
  do {
    iVar2 = iVar1 * 0x18 + param_1;
    *(undefined2 *)(iVar2 + 0xe) = 0xffff;
    *(undefined2 *)(iVar2 + 0x10) = 0xffff;
    iVar1 = iVar1 + 1;
  } while (iVar1 < 10);
  return CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:ec94  FUN_1010_ec94
   ============================================================ */

/* WARNING: Removing unreachable block (ram,0x1010ecca) */

undefined2 __stdcall16far
FUN_1010_ec94(int param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  
  lVar4 = FUN_1010_ed28(param_3,param_4);
  iVar2 = 0;
  while( true ) {
    if (9 < iVar2) {
      return 0;
    }
    lVar5 = FUN_1010_ed28(iVar2 * 0x18 + param_1,param_2);
    if (lVar5 < lVar4) break;
    iVar2 = iVar2 + 1;
  }
  for (iVar3 = 9; iVar2 < iVar3; iVar3 = iVar3 + -1) {
    iVar1 = iVar3 * 0x18 + param_1;
    FUN_1000_1e38(iVar1,param_2,iVar1 + -0x18,param_2,0x18);
  }
  FUN_1000_1e38(iVar2 * 0x18 + param_1,param_2,param_3,param_4,0x18);
  return 1;
}



/* ============================================================
   1010:ed28  FUN_1010_ed28
   ============================================================ */

undefined4 __stdcall16far FUN_1010_ed28(undefined4 param_1)

{
  int iVar1;
  undefined2 uVar2;
  bool bVar3;
  undefined2 local_6;
  undefined2 local_4;
  
  uVar2 = (undefined2)((ulong)param_1 >> 0x10);
  iVar1 = (int)param_1;
  local_6 = *(uint *)(iVar1 + 0xe);
  local_4 = *(int *)(iVar1 + 0x10);
  if (*(int *)(iVar1 + 0x16) == 1) {
    bVar3 = 0x697f < local_6;
    local_6 = local_6 + 0x9680;
    local_4 = local_4 + 0x98 + (uint)bVar3;
  }
  if (*(int *)(iVar1 + 0x14) == 1) {
    bVar3 = 0xb4bf < local_6;
    local_6 = local_6 + 0x4b40;
    local_4 = local_4 + 0x4c + (uint)bVar3;
  }
  return CONCAT22(local_4,local_6);
}



/* ============================================================
   1010:ed68  FUN_1010_ed68
   ============================================================ */

undefined2 * __stdcall16far FUN_1010_ed68(undefined2 *param_1,undefined2 param_2)

{
  *param_1 = 0x26;
  param_1[1] = 0x28;
  param_1[2] = 0x27;
  param_1[3] = 0x25;
  param_1[4] = 0x21;
  param_1[5] = 0x24;
  param_1[6] = 0x22;
  param_1[7] = 0x23;
  param_1[8] = 0xc;
  param_1[9] = 0x41;
  param_1[10] = 0x5a;
  param_1[0xb] = 0x46;
  param_1[0xc] = 0x50;
  param_1[0xd] = 0x4d;
  param_1[0xe] = 0x51;
  param_1[0xf] = 0x53;
  param_1[0x10] = 0x54;
  param_1[0x11] = 0x55;
  param_1[0x12] = 0x44;
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:edea  FUN_1010_edea
   ============================================================ */

undefined2 * __stdcall16far
FUN_1010_edea(undefined2 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  undefined2 uVar2;
  int iVar3;
  int extraout_DX;
  undefined2 uVar4;
  undefined2 unaff_SS;
  undefined4 uVar5;
  undefined1 local_18e [2];
  int local_18c;
  undefined1 local_106 [128];
  undefined1 local_86 [128];
  int local_6;
  int iStack_4;
  
  FUN_1010_ec36(param_1 + 3,param_2);
  FUN_1010_ed68(param_1 + 0x7b,param_2);
  param_1[1] = param_3;
  param_1[2] = param_4;
  *param_1 = 0;
  param_1[0x8f] = 0;
  param_1[0x8e] = 0;
  param_1[0x91] = 0;
  param_1[0x90] = 0;
  param_1[0x93] = 0;
  param_1[0x92] = 0;
  GETMODULEFILENAME(0x1010,0x80,local_86,unaff_SS);
  iVar3 = extraout_DX;
  iVar1 = FUN_1000_1e04(local_86);
  _local_6 = CONCAT22(iVar3,iVar1);
  if (iVar3 != 0 || iVar1 != 0) {
    *(undefined1 *)(iVar1 + 1) = 0;
    FUN_1000_16aa(local_106);
    FUN_1000_1656(local_106);
  }
  uVar5 = OPENFILE(0x1000,0x4000,local_18e,unaff_SS,local_106);
  uVar4 = (undefined2)((ulong)uVar5 >> 0x10);
  if ((int)uVar5 == -1) {
    if (local_18c != 2) {
      *param_1 = 1;
      FUN_1008_2d02(param_1[1],param_1[2],0,0x374,0x1018);
      goto LAB_1010_ef28;
    }
    uVar2 = FUN_1000_1a02(0,0);
    param_1[0x92] = uVar2;
    param_1[0x93] = uVar4;
    iVar3 = FUN_1018_00ea(param_1,param_2);
    if (iVar3 == 0) {
      *param_1 = 1;
      goto LAB_1010_ef28;
    }
  }
  FUN_1018_0000(param_1,param_2);
LAB_1010_ef28:
  return (undefined2 *)CONCAT22(param_2,param_1);
}



/* ============================================================
   1010:ef2e  FUN_1010_ef2e
   ============================================================ */

void __stdcall16far FUN_1010_ef2e(void)

{
  return;
}



/* ============================================================
   1018:0000  FUN_1018_0000
   ============================================================ */

bool __stdcall16far FUN_1018_0000(undefined2 *param_1,undefined2 param_2)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  int extraout_DX;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  bool bVar4;
  undefined1 local_190 [136];
  undefined1 local_108 [128];
  undefined1 local_88 [130];
  undefined2 local_6;
  int local_4;
  
                    /* Segment:    4
                       Offset:     00020ee0
                       Length:     03ee
                       Min Alloc:  03ee
                       Flags:      1d50
                           Code
                           Discardable
                           Moveable
                           Preload
                           Impure (Non-shareable)
                        */
  GETMODULEFILENAME(unaff_CS,0x80,local_88,unaff_SS);
  iVar3 = extraout_DX;
  iVar2 = FUN_1000_1e04(local_88);
  local_4 = iVar3;
  if (iVar3 != 0 || iVar2 != 0) {
    *(undefined1 *)(iVar2 + 1) = 0;
    FUN_1000_16aa(local_108);
    FUN_1000_1656(local_108);
  }
  iVar3 = OPENFILE(0x1000,0,local_190,unaff_SS,local_108);
  bVar4 = iVar3 != -1;
  local_4 = iVar3;
  if (bVar4) {
    uVar1 = param_1[1];
    local_6 = param_1[2];
    _LREAD(0x11d0,0x128,param_1,param_2);
    _LCLOSE(0x11d0,local_4,iVar3);
    param_1[1] = uVar1;
    param_1[2] = local_6;
  }
  else {
    FUN_1008_2d02(param_1[1],param_1[2],0,0x39e,0x1018);
    *param_1 = 1;
  }
  return bVar4;
}



/* ============================================================
   1018:00ea  FUN_1018_00ea
   ============================================================ */

bool __stdcall16far FUN_1018_00ea(undefined2 *param_1)

{
  int iVar1;
  int iVar2;
  int extraout_DX;
  undefined2 *puVar3;
  undefined2 uVar4;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined1 local_18e [136];
  undefined1 local_106 [128];
  undefined1 local_86 [130];
  int local_4;
  
  uVar4 = (undefined2)((ulong)param_1 >> 0x10);
  puVar3 = (undefined2 *)param_1;
  GETMODULEFILENAME(unaff_CS,0x80,local_86,unaff_SS);
  iVar2 = extraout_DX;
  iVar1 = FUN_1000_1e04(local_86);
  local_4 = iVar2;
  if (iVar2 != 0 || iVar1 != 0) {
    *(undefined1 *)(iVar1 + 1) = 0;
    FUN_1000_16aa(local_106);
    FUN_1000_1656(local_106);
  }
  iVar2 = OPENFILE(0x1000,0x1001,local_18e,unaff_SS,local_106);
  if (iVar2 != -1) {
    iVar1 = iVar2;
    _LWRITE(0x11d0,0x128,puVar3,uVar4);
    _LCLOSE(0x11d0,iVar2,iVar1);
  }
  else {
    FUN_1008_2d02(puVar3[1],puVar3[2],0,0x3c6,0x1018);
    *param_1 = 1;
  }
  return iVar2 != -1;
}



/* ============================================================
   1018:01aa  FUN_1018_01aa
   ============================================================ */

undefined4 __stdcall16far FUN_1018_01aa(int *param_1,undefined2 param_2)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  if (*param_1 != 1) {
    puVar1 = (uint *)(param_1 + 0x8e);
    uVar2 = *puVar1;
    *puVar1 = *puVar1 + 1;
    param_1[0x8f] = param_1[0x8f] + (uint)(0xfffe < uVar2);
    iVar3 = param_1[0x8e];
    iVar5 = param_1[0x8f];
    iVar4 = FUN_1018_00ea(param_1,param_2);
    if (iVar4 != 0) goto LAB_1018_01f8;
    *param_1 = 1;
  }
  iVar3 = -1;
  iVar5 = -1;
LAB_1018_01f8:
  return CONCAT22(iVar5,iVar3);
}



/* ============================================================
   1018:01fe  FUN_1018_01fe
   ============================================================ */

undefined4 __stdcall16far FUN_1018_01fe(int *param_1,undefined2 param_2)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  if (*param_1 != 1) {
    puVar1 = (uint *)(param_1 + 0x90);
    uVar2 = *puVar1;
    *puVar1 = *puVar1 + 1;
    param_1[0x91] = param_1[0x91] + (uint)(0xfffe < uVar2);
    iVar3 = param_1[0x90];
    iVar5 = param_1[0x91];
    iVar4 = FUN_1018_00ea(param_1,param_2);
    if (iVar4 != 0) goto LAB_1018_024c;
    *param_1 = 1;
  }
  iVar3 = -1;
  iVar5 = -1;
LAB_1018_024c:
  return CONCAT22(iVar5,iVar3);
}



/* ============================================================
   1018:0252  FUN_1018_0252
   ============================================================ */

/* WARNING: Removing unreachable block (ram,0x101802b2) */

undefined2 __stdcall16far
FUN_1018_0252(int *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
             undefined2 param_5,undefined2 param_6,undefined2 param_7)

{
  undefined2 *puVar1;
  int iVar2;
  undefined2 unaff_SS;
  long lVar3;
  long lVar4;
  undefined1 local_1a [14];
  undefined2 local_c;
  undefined2 local_a;
  undefined2 local_8;
  undefined2 local_6;
  undefined2 local_4;
  
  if (*param_1 != 1) {
    FUN_1010_ec10(local_1a,unaff_SS);
    local_c = param_6;
    local_a = param_7;
    local_8 = param_5;
    local_6 = param_4;
    local_4 = param_3;
    lVar3 = FUN_1010_ed28(local_1a,unaff_SS);
    lVar4 = FUN_1010_ed28(param_1 + 0x6f,param_2);
    if (lVar4 < lVar3) {
      puVar1 = *(undefined2 **)(param_1 + 1);
      FUN_1008_407e(*(undefined2 *)((int)*(undefined4 *)((undefined2 *)puVar1 + 6) + 8),*puVar1,
                    local_1a);
      iVar2 = FUN_1010_ec94(param_1 + 3,param_2,local_1a,unaff_SS);
      if (iVar2 != 0) {
        iVar2 = FUN_1018_00ea(param_1,param_2);
        if (iVar2 != 0) {
          return 1;
        }
        *param_1 = 1;
      }
    }
  }
  return 0;
}



/* ============================================================
   1018:0310  FUN_1018_0310
   ============================================================ */

bool __stdcall16far
FUN_1018_0310(int *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  
  iVar1 = *param_1;
  if (iVar1 != 1) {
    FUN_1000_1e38(param_3,param_4,param_1 + 3,param_2,0xf0);
  }
  return iVar1 != 1;
}



/* ============================================================
   1018:0342  FUN_1018_0342
   ============================================================ */

bool __stdcall16far
FUN_1018_0342(int *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  int iVar1;
  
  iVar1 = *param_1;
  if (iVar1 != 1) {
    FUN_1000_1e38(param_3,param_4,param_1 + 0x7b,param_2,0x26);
  }
  return iVar1 != 1;
}



/* ============================================================
   11d0:0000  SNDPLAYSOUND
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SNDPLAYSOUND(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0004  FATALEXIT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void FATALEXIT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0008  GETVERSION
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETVERSION(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:000c  GLOBALALLOC
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GLOBALALLOC(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0010  GLOBALREALLOC
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GLOBALREALLOC(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0014  GLOBALLOCK
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GLOBALLOCK(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0018  GLOBALUNLOCK
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GLOBALUNLOCK(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:001c  GLOBALSIZE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GLOBALSIZE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0020  LOCKSEGMENT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void LOCKSEGMENT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0024  WAITEVENT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void WAITEVENT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0028  GETMODULEFILENAME
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETMODULEFILENAME(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:002c  MAKEPROCINSTANCE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void MAKEPROCINSTANCE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0030  FREEPROCINSTANCE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void FREEPROCINSTANCE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0034  FINDRESOURCE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void FINDRESOURCE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0038  LOADRESOURCE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void LOADRESOURCE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:003c  LOCKRESOURCE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void LOCKRESOURCE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0040  FREERESOURCE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void FREERESOURCE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0044  OPENFILE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void OPENFILE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0048  _LCLOSE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void _LCLOSE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:004c  _LREAD
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void _LREAD(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0050  _LWRITE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void _LWRITE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0054  INITTASK
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void INITTASK(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0058  DOS3CALL
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void DOS3CALL(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:005c  __AHINCR
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void __AHINCR(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0060  GETDOSENVIRONMENT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETDOSENVIRONMENT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0064  FATALAPPEXIT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void FATALAPPEXIT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0068  __WINFLAGS
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void __WINFLAGS(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:006c  SETBKCOLOR
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SETBKCOLOR(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0070  SETBKMODE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SETBKMODE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0074  SETTEXTCOLOR
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SETTEXTCOLOR(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0078  LINETO
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void LINETO(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:007c  MOVETO
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void MOVETO(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0080  RECTANGLE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void RECTANGLE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0084  TEXTOUT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void TEXTOUT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0088  BITBLT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void BITBLT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:008c  SELECTOBJECT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SELECTOBJECT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0090  CREATECOMPATIBLEDC
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void CREATECOMPATIBLEDC(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0094  CREATEFONT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void CREATEFONT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0098  CREATEPEN
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void CREATEPEN(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:009c  DELETEDC
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void DELETEDC(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00a0  DELETEOBJECT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void DELETEOBJECT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00a4  GETSTOCKOBJECT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETSTOCKOBJECT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00a8  GETTEXTEXTENT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETTEXTEXTENT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00ac  GETTEXTMETRICS
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETTEXTMETRICS(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00b0  MESSAGEBOX
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void MESSAGEBOX(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00b4  INITAPP
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void INITAPP(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00b8  POSTQUITMESSAGE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void POSTQUITMESSAGE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00bc  SETTIMER
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SETTIMER(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00c0  KILLTIMER
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void KILLTIMER(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00c4  SETCAPTURE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SETCAPTURE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00c8  RELEASECAPTURE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void RELEASECAPTURE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00cc  SETFOCUS
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SETFOCUS(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00d0  GETWINDOWRECT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETWINDOWRECT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00d4  GETCLIENTRECT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETCLIENTRECT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00d8  GETWINDOWTEXT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETWINDOWTEXT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00dc  BEGINPAINT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void BEGINPAINT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00e0  ENDPAINT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void ENDPAINT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00e4  CREATEWINDOW
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void CREATEWINDOW(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00e8  SHOWWINDOW
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SHOWWINDOW(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00ec  MOVEWINDOW
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void MOVEWINDOW(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00f0  REGISTERCLASS
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void REGISTERCLASS(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00f4  GETDC
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETDC(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00f8  RELEASEDC
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void RELEASEDC(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:00fc  SETRECT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SETRECT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0100  PTINRECT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void PTINRECT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0104  INTERSECTRECT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void INTERSECTRECT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0108  DRAWICON
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void DRAWICON(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:010c  DIALOGBOX
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void DIALOGBOX(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0110  ENDDIALOG
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void ENDDIALOG(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0114  GETDLGITEM
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETDLGITEM(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0118  SENDDLGITEMMESSAGE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void SENDDLGITEMMESSAGE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:011c  DEFWINDOWPROC
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void DEFWINDOWPROC(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0120  GETMESSAGE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETMESSAGE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0124  PEEKMESSAGE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void PEEKMESSAGE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0128  POSTMESSAGE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void POSTMESSAGE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:012c  TRANSLATEMESSAGE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void TRANSLATEMESSAGE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0130  DISPATCHMESSAGE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void DISPATCHMESSAGE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0134  UPDATEWINDOW
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void UPDATEWINDOW(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0138  INVALIDATERECT
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void INVALIDATERECT(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:013c  GETSYSTEMMENU
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETSYSTEMMENU(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0140  LOADCURSOR
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void LOADCURSOR(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0144  LOADICON
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void LOADICON(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0148  LOADBITMAP
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void LOADBITMAP(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:014c  GETSYSTEMMETRICS
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETSYSTEMMETRICS(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0150  GETASYNCKEYSTATE
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETASYNCKEYSTATE(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0154  REDRAWWINDOW
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void REDRAWWINDOW(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0158  APPENDMENU
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void APPENDMENU(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:015c  DESTROYICON
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void DESTROYICON(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0160  GETOPENFILENAME
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETOPENFILENAME(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0164  GETSAVEFILENAME
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void GETSAVEFILENAME(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ============================================================
   11d0:0168  COMMDLGEXTENDEDERROR
   ============================================================ */

/* WARNING: Control flow encountered bad instruction data */

void COMMDLGEXTENDEDERROR(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


