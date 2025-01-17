#pragma once

#include "common/common.h"
#include "resource_files/image_resource_file.h"
#include "resource_files/image_cube_resource_file.h"

#include "imgui.h"
#include "imgui_stdlib.h"

class Entity;

#define FONT_ICON_BUFFER_NAME_ROOTEX ROOTEX_compressed_data_base85
#define FONT_ICON_BUFFER_SIZE_ROOTEX 0x1527

#define ICON_MIN_ROOTEX 0xf002
#define ICON_MAX_ROOTEX 0xf2d3

#define ICON_ROOTEX_BOOKMARK u8"\uf02e"
#define ICON_ROOTEX_CHECK u8"\uf00c"
#define ICON_ROOTEX_CLOCK_O u8"\uf017"
#define ICON_ROOTEX_CLOUD u8"\uf0c2"
#define ICON_ROOTEX_DATABASE u8"\uf1c0"
#define ICON_ROOTEX_EXTERNAL_LINK u8"\uf08e"
#define ICON_ROOTEX_FILE u8"\uf15b"
#define ICON_ROOTEX_FILE_AUDIO_O u8"\uf1c7"
#define ICON_ROOTEX_FILE_CODE_O u8"\uf1c9"
#define ICON_ROOTEX_FILE_IMAGE_O u8"\uf1c5"
#define ICON_ROOTEX_FILE_TEXT u8"\uf15c"
#define ICON_ROOTEX_FILES_O u8"\uf0c5"
#define ICON_ROOTEX_FLOPPY_O u8"\uf0c7"
#define ICON_ROOTEX_FOLDER u8"\uf07b"
#define ICON_ROOTEX_FOLDER_OPEN u8"\uf07c"
#define ICON_ROOTEX_FONT u8"\uf031"
#define ICON_ROOTEX_FORT_AWESOME u8"\uf286"
#define ICON_ROOTEX_MINUS u8"\uf068"
#define ICON_ROOTEX_MINUS_CIRCLE u8"\uf056"
#define ICON_ROOTEX_PENCIL_SQUARE_O u8"\uf044"
#define ICON_ROOTEX_PICTURE_O u8"\uf03e"
#define ICON_ROOTEX_PLUS u8"\uf067"
#define ICON_ROOTEX_REFRESH u8"\uf021"
#define ICON_ROOTEX_REPEAT u8"\uf01e"
#define ICON_ROOTEX_SEARCH u8"\uf002"
#define ICON_ROOTEX_WINDOW_CLOSE u8"\uf2d3"

inline static const char FONT_ICON_BUFFER_NAME_ROOTEX[5415 + 1] = "7])#######R9c$c'/###O@?>#+lQS%Ql#v#X^@iFuP:Ul,7###cP'##4<U=B;w.GM]cFe-]br-$KQshF=I&##-`'##?V`w'<op0FJVcenbkVw02)[w'75=RDL>5$Y+)###nIq-$Mu@['"
                                                                  "dnro8SfH_&UKo-$jZIkE=)AU]*s)##tf+##8CC0Fh6D.Iof%##tWn-$f`FkOQ&###(Df$SodPirc[4gL)>$##4+0oQ87,F%t$pOoE7np%xF[w'OMYY#N$H&#4$aw$E5x-#LAw,vmwmo%"
                                                                  "L0vhLE;K;-M_@U.Mlu##VCDX-3on34wdJe$_/_f1*1L'#A2nu5TP'##h#k-$1L?v$'r#Z$Gp4R*%ej-$tln'N;:#gL*8T;-,r:T.,*&##SvR/MBP#<-*?:AQdcOkLdn_d-'Ok?KVl:;$"
                                                                  "+2[W%V+6;-7k+/(i@H0':#3t,T'<*2u=E67JP(t>?k30KVVPZb=*09d$_w#b?099dXKYBN?TW:v>[Fs2o[njJX[I;@94d?KGVtA#>:.N#V;V`3Y,cD4#>B;-H:X3.UxefL&i)[$0u68%"
                                                                  "47no%8ONP&<h/2'@*gi'DBGJ(HZ(,)Ls_c)P5@D*TMw%+Vb$iP)LF_&<4Z$9RAl-$rB*##D,'##Q@m-$YC)##wh(##e-%##0)l-$p.go7KD:&5QC[2L7k]m&Lap?dPIHT.s)M/qGLS#c"
                                                                  "Jc+$,6LC_&cn^-6w88-#e,O1#@QY6#rvd;#h5O1#a1<)#>wN=$3LF84`$At[:^/@66RG##Pui/%Iq8;]`>(C&Okk2$7U$Z$AGs8].#KxtA:u+M`>oO(FaKZ-<IXD#`xeH#'K%L$[8+)6"
                                                                  "73j#MOP?uuYg8e$UNc##ci*N.S9F&#%rBB#-<Tv-'[/I$YUw8/(S/C4,dI)*fpB:%jX8/LTnLuP$]H8%KxU8%SL9Y.]#?sjlh0(alFPR#H9OA#'_JxJMqW^#=UlA#Xu2U/#=kL;s9auJ"
                                                                  "j(,bFlsL:Q]8)YYI,>>#QESQ'CZ35]prc'&>L)=%&Y@C#i_Z=7No^I*7nJ,2Wt+N'GM1eN47DcNF<w%,(o'B#`//;MJ)Nfq57x:.3.v(EL.o&,pRpq#0[Mm0&,###Tx^##hZGG)weu21"
                                                                  "5nOJ(rxwZ-TekD#u3YD#D?cD3&L,/(+gnP'SgOA#4W-/(n5pV._'3wp4L0/LXi?%bdDDO'eHqT7:].GV3c*ci>&BPtH4$Y#X.pFVu2lFV#^-2B)+,2B3^Z+MjsO?-[g-]-s/f'/1)bq2"
                                                                  "1PJgOcW^Jfdf/oNE2jeNCp<9/WH5<.jdUG)SwC.3SV>c4>A^Y,5+r;$0$^/NWftK3RVx_47$:u$c,g[uZM8L(+]tfd%(Y4=%<D[DkG,@%+hm&OM0W&F6p2AUPWwJJ4A-#,h=6,)W$650"
                                                                  "N,#Yuwlip/'AG4Oqxdofor[KDBJ*p%..m7&;e,@06,<D#M_MjF:TfM(65YY#@$2W.=4O3XvFDZ#5D-W.$;Ls-oUFb35IL,34R(hLri4,/sJ))3E[3TBWhZLMrAo8%OHwm0TAOZ6W.PV-"
                                                                  "=@BQ/XX48nw8<cDMa]iuium9)igAE+%]v7/gu%t]:U.g9wCo)*sK#(+t?[q)uKeU)7xO-)V&dN#3BQG)uP/O45FMo$n2/tu-i(UL+0IQ1p`b'OMk8hGn2R.)RcVBocTPDOhfRq.DUsl&"
                                                                  "$&G6j)B('#nhAg1paUV$kxw%#n#]I*/=_hLfM]s$Cr&t%#M,W-d9'hlM2'J35i4f)_Y_Z-Mx;=.Z&f.:a[xw'2q'Y$G38$5Zs<7.;G(<-$87V?M42X-n+[w'1q-[B<.-W6)8[0#$UG+#"
                                                                  "YQ9)3+2pb4kMYx6sDB@#AGO.)FR/bIKBIIMee,g)u<*u-=)r>$IE(E##2J&4XkRP/fTqc)6^UD3jpB:%d^?mSTn;Z#?\?VSfBUh#?YeXD*D@T:/6$m`*'8_oee[5n'e&sk'+USt$MwPo7"
                                                                  "+_Nj/n/MD*Vd,3'e++G#5f#MCBk=K;d>`Y5uS]p.K4#2gga*Z#9^oP<R,.-)):DGDe7i9%:W?bur5/)kP&*k'*r,0)MWP/(+gc<.M(I8%G'kT%Eca4$L/`Lp.o'j(odJR/Zp^U%uJA3/"
                                                                  "'&>uuv#Fwp`?N$#p8q'#=''u$+dfF43c?X-$X>r%n4Rv$t@0+*5HSF4o=]:/Z0cw$H:u>5bF%SVSaDL$f%*L#[i:##5vg$bdD24'FxLe$0m.?8)K+<A`R)T7->S/(er]&GwTUo7bL$X."
                                                                  "el24'?mXZ->Yi638]8+M_l*?8PJU[-2^rQNRhrr$L9[0#7xL$#EDW)#m,>>#H,Jd)<f-5/GgZ)*.Wae)cL:)NXGL+*atr?#Df6<.oN^Z6CE(E#JfffL5ZdO(d/Z]4V)ZA#tYUSIZf'#B"
                                                                  "V(kM(KB*=-::m/=WF1%bfS'JI1IvG4DU/m&659F*vwMt_`4`BS^HWp'xm@;-b/T=l4_lcuxcPc<JKBA#p:^?.)qK`aid?Au'L:@9-j'8+#R:hLC=Ss$;;mw9R1pgLJ[A[#$_^UunPa^#"
                                                                  "ha@;-dNo?9M-%?I]'WV$^)V$#?,>>#-28hcNxko7u1vAms-]`abi#>#NgAE+$J,W-)[4Qh@.YS7qI`.Mugji0_Z==-?9Fk3w758.9<hc),39Z-$_Aj0LNv)4:6a'%3JQ590@Si)H&B.*"
                                                                  "_bfr/8Yer/C?x_ur>l(5r@1XMK@/GVQUQ;-***:2I)/B#Rr[fL58,,M.[K=9Cn]G3t&V`tW_gC-sEENMlQMA.],>>#dDov$)8[0#ATS_%MbIW-UU#I6X9dTMn2:Z@4=3H#(+_^#Rs).$"
                                                                  "Uqvw'6T9@MSGOJME]W)3,aLv>gQ,,)ML$##@=)?#-<3jLBV8f3EPsD#U[YB#,5Rv$g&?a3EC2=%Ib7o[(f/dtZ9R%-Uwe'&c:]#.S]M4uNDKm6-_r>#s'C@-NJ6/:.N,VH+QB],H0exO"
                                                                  "-@hB#P%19:Kxu;$OcB1PtlIfLQnd##P.HP/w4YYhK._58:<>)4j1.Q/OZ'u$wkH>#FIP8/Np%T/V_>l$b6`B8_U?_#l_[h(]Lw;8%?&'$kw0<f^+hB/otJPuq9v7&V;`Z#DX57/Yj;A+"
                                                                  "Ug7dus&9W-FTq$0)M?W-,fp?9Dkcbu/GYk4ltaD+5o6d3@,R&Q-LT/LoD[]+S(4GM2u%-#OM>c4^RS(#NMfD**^B.*rC>Z,80FKl9,U__u38JH'X<3hTCru5vh2@SxR<P#&]DV#D,#Rj"
                                                                  "YP50h:(*mPXe;7;Rl=9#(&>uu0oqr$B3=&#kWt&#=8E)#KV(R<JQ+<-H%9k$JQ1K1^j%J3U>Ds%YWA@#TGp'/V),d3#<7'4*H3'5l%>o7cXI=lsf5',9Q[oA8A8U)K685L$5qi0-ksJ1"
                                                                  "$7E`uXd;^uX>dl/9-KC&/&w<($]%Q/O?cr/N]tpASH9_849M*0%=Kq#M$kpu[EaD#PUhpL)UxK#SrW,u&?)qMS7$##?4hWJW[?##vJ6(#<A)20g?dB#m=]:/%E6C#o7+n-Bis9)%%x[-"
                                                                  "YSjA#dp7m0&q)b$aJ))3W%@<A.gg@#o]r.L$px=#;`*U)tDtM&nbbr/BV4'5J>jM-ap`M/n;64'3j-IMw'^fL)V/+MlEcr/C+>@2+sUB#&Da-$B&G:&[`IH)U<Xp.l(.GV@AVHm1mNS7"
                                                                  "V?Q]u7L,W-8rP9iU@oZ$Fibp/CCg;,OseQ-G%+n-WN3L#BiIe*5nOJ(h5.)*jr/+*nPcd$1I=j0&JH##6^TlS;4T=Y<gm>-9pEUMwFH##o-'O'Y&R**Jx%ctq3PG-L5r#8U'9.?RAnx4"
                                                                  "KVR'A>qwX-)#F#$?L3N(PH&49qRkM(LF-Z$OTh`._w2n8dxl@0DlJO',49h(eYFK3A_D9.pUO**#L:Au?NaM-'*,k-UZUh5xUkA#fhlp7?BZp0ljE$#hpB'#]####_DTi)boX;-VekD#"
                                                                  "]3W9.[sFA#<X&f3pO@@#i`UHI9.vjuE%[Ef,k%wUhM)R#Kj/?f&Aw8f&xu+>$.X>9vTt]YTU8SO&_NM044h>$f9]>-(:]jLb&?E&&H<A+q^EM0Ue$##1Qjb$4W1HOU_w[-40_:%MxMT/"
                                                                  "]5QJ(SR'Z-C0p,2R^g@#NEZd3aIcI))oio/%VET%UQspA=KGSIULS:d=vRdu75]CuOq$0ufu'L#>Y[OVk1k3G(kZoA`thB8TIbr/agM%O1Q;>5@R<=$/rVB#kwF>#euita<dF*GJ4$##"
                                                                  "e/-e%][*B5#[#5AcIS-*edB:%HXX/2weA?R6[XD#_Uvr-VJh*%WA;9/2,h.*hZt9%=_tt(tX<9/K&<X(S/<tiKmCloW_.<$e$M=MdDDO'_.+@#k<Xe)W:S9CcfWI)mBqWLUD&J)*sVb3"
                                                                  "=l?5/p$&)?5LWo%?5JX%./%A*3F5^XhRub'?h@?uj;W'4H#sr6O=*P(DRt-VBN[q*fNj'4>PxM5=YM^#CwT4AJbBW/:1o(#a?H#8Itw5/-@XA#R4r?#fgGj'x%MT/ObE]-QtfM'-Wgf1"
                                                                  "6HSP/kK(E#_3rhLE)t1)ter0,%hGZ.-1i^%11uU&FIJa^d/Gb%QIsYlG4`;$]Ya7Q.sc7Iw<6AO2CL97tOv51jv-tLEWOjL$:HErwNw5/*vwA#;nn0#Q*66Q`_r7%xcE#$+PY8&w@o%u"
                                                                  "_4D'7&,###T%J/L$]VP&c(n1KY^#&4P4vr-;`tw5[$wDOmSvJ(Z=]:/9U$.MA?FV.0ImO(YV#<-rjCa3W&WF3oV8f3bR(f)4JJC4'cXF3`c&gLq[i8.[GUv-R#SF4VO^-6P-9_8IA*F3"
                                                                  "2=L^#$)A9(vX0^#,4Vp.^I%SV+T+7*G&5^$GL%@#]f6[-VNeg)#/u<6FC*e+e1in/nqs9%9st_/c&E^#XUL'Od;-J(%h:+GN1B%b6?q0(3gErLCSJ874SY>#g13x'HKX>-(,&L([;`0("
                                                                  "w(2k'boK>#NQA#GVV(3(2%32Gdm(]$_fUV$rLb&#&sgo.ITR12&fuS.&Cs?#KcuS.^#YA#-uU5/V?(E#'_PI<[<>n_pPY=$bu=6&a'A6&^+R.M<d7iLJp>h19a-?8Ts&<A4(g`a3R.hS"
                                                                  "')@]-'#&4;Q=6##Gt%NK?,>>#*AY>#.Ml>#2Y(?#6f:?#9i1$#x#jW#>(`?#B4r?#F@.@#JL@@#NXR@#Ree@#[HTu$%QcdGWQ/jBksaNEjquhF9d7bFr'Q-GfDZD%)p:eGm;__%%/$PE"
                                                                  "%m>PEl-vLF%DHoD8LofG4'Zu'vm@uBD5O$HDq16M58('F9C4,H:%VeG_Wb'%u2J:C6sXdM<-5bFb1)7'02YVC'6kjE?r>LF-k=nE7vtB%p)fUC;=F`%+SV.GT88bF01v<-*G#W-w;q92"
                                                                  "pYV<Bh[G<B7HFHM/6w0F=^M'J=L+6B+dYhFO`=HM1wvgF_P,]5Q%fV1*SM*Hr;Zw&,-]:CEqFnB-gOVC(PUV$+oe,qa%###";

void RootexFPSGraph(const char* name, Vector<float>& fpsRecords, float lastFPS);
void RootexSelectableImage(const char* name, ImageResourceFile* image, Function<void(const String&)> onSelected);
void RootexSelectableImageCube(const char* name, ImageCubeResourceFile* image, Function<void(const String&)> onSelected);
