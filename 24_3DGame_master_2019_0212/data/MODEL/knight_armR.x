xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 290;
 -1.22003;2.55870;2.52975;,
 -1.14560;3.34462;2.52975;,
 -3.15320;3.53475;2.52975;,
 -3.22763;2.74882;2.52975;,
 -1.07117;4.13054;2.52975;,
 -3.07877;4.32067;2.52975;,
 -5.16080;3.72487;2.52975;,
 -5.23523;2.93895;2.52975;,
 -5.08637;4.51080;2.52975;,
 -6.49609;3.85133;0.00272;,
 -6.57052;3.06541;0.00272;,
 -6.42165;4.63726;0.00272;,
 -5.16080;3.72487;-2.52430;,
 -5.23523;2.93895;-2.52430;,
 -5.08637;4.51080;-2.52430;,
 -3.15320;3.53475;-2.52430;,
 -3.22763;2.74882;-2.52430;,
 -3.07877;4.32067;-2.52430;,
 -1.14560;3.34462;-2.52430;,
 -1.22003;2.55870;-2.52430;,
 -1.07117;4.13054;-2.52430;,
 0.18969;3.21816;0.00272;,
 0.11526;2.43224;0.00272;,
 0.26412;4.00409;0.00272;,
 -1.14560;3.34462;2.52975;,
 -1.22003;2.55870;2.52975;,
 -1.07117;4.13054;2.52975;,
 0.26412;4.00409;0.00272;,
 -1.07117;4.13054;-2.52430;,
 -3.07877;4.32067;-2.52430;,
 -3.07877;4.32067;0.00272;,
 -5.08637;4.51080;-2.52430;,
 -6.42165;4.63726;0.00272;,
 -1.22003;2.55870;-2.52430;,
 0.11526;2.43224;0.00272;,
 -3.22763;2.74882;0.00272;,
 -3.22763;2.74882;-2.52430;,
 -6.57052;3.06541;0.00272;,
 -5.23523;2.93895;-2.52430;,
 -0.04023;0.45318;-0.25653;,
 -0.04023;0.26136;-0.44836;,
 -0.04023;0.52340;-0.90223;,
 -0.04023;0.90705;-0.51857;,
 -0.04023;-0.00068;-0.51857;,
 -0.04023;-0.00068;-1.04266;,
 -0.04023;-0.26272;-0.44836;,
 -0.04023;-0.52476;-0.90223;,
 -0.04023;-0.45455;-0.25653;,
 -0.04023;-0.90841;-0.51857;,
 -0.04023;-0.52476;0.00551;,
 -0.04023;-1.04884;0.00551;,
 -0.04023;-0.45455;0.26755;,
 -0.04023;-0.90841;0.52958;,
 -0.04023;-0.26272;0.45937;,
 -0.04023;-0.52476;0.91323;,
 -0.04023;-0.00068;0.52958;,
 -0.04023;-0.00068;1.05365;,
 -0.04023;0.26136;0.45937;,
 -0.04023;0.52340;0.91323;,
 -0.04023;0.45318;0.26755;,
 -0.04023;0.90705;0.52958;,
 -0.04023;0.52340;0.00551;,
 -0.04023;1.04747;0.00551;,
 -0.04023;0.78543;-1.35610;,
 -0.04023;1.36091;-0.78061;,
 -0.04023;-0.00068;-1.56674;,
 -0.04023;-0.78680;-1.35610;,
 -0.04023;-1.36228;-0.78061;,
 -0.04023;-1.57292;0.00551;,
 -0.04023;-1.36228;0.79162;,
 -0.04023;-0.78680;1.36709;,
 -0.04023;-0.00068;1.57773;,
 -0.04023;0.78543;1.36709;,
 -0.04023;1.36091;0.79162;,
 -0.04023;1.57155;0.00551;,
 -0.04023;1.04749;-1.80998;,
 -0.04023;1.81480;-1.04267;,
 -0.04023;-0.00068;-2.09084;,
 -0.04023;-1.04885;-1.80998;,
 -0.04023;-1.81617;-1.04267;,
 -0.04023;-2.09702;0.00551;,
 -0.04023;-1.81617;1.05367;,
 -0.04023;-1.04885;1.82099;,
 -0.04023;-0.00068;2.10184;,
 -0.04023;1.04749;1.82099;,
 -0.04023;1.81480;1.05367;,
 -0.04023;2.09566;0.00551;,
 -1.24441;2.26864;-1.30470;,
 -1.24441;1.30951;-2.26383;,
 -2.70384;1.35962;-2.35062;,
 -2.70384;2.35543;-1.35481;,
 -1.24441;-0.00068;-2.61489;,
 -2.70384;-0.00068;-2.71511;,
 -1.24441;-1.31088;-2.26383;,
 -2.70384;-1.36098;-2.35062;,
 -1.24441;-2.27001;-1.30470;,
 -2.70384;-2.35679;-1.35481;,
 -1.24441;-2.62108;0.00551;,
 -2.70384;-2.72129;0.00551;,
 -1.24441;-2.27001;1.31569;,
 -2.70384;-2.35679;1.36580;,
 -1.24441;-1.31088;2.27482;,
 -2.70384;-1.36098;2.36161;,
 -1.24441;-0.00068;2.62589;,
 -2.70384;-0.00068;2.72611;,
 -1.24441;1.30951;2.27482;,
 -2.70384;1.35962;2.36161;,
 -1.24441;2.26864;1.31569;,
 -2.70384;2.35543;1.36580;,
 -1.24441;2.61971;0.00551;,
 -2.70384;2.71992;0.00551;,
 -1.24441;2.26864;-1.30470;,
 -2.70384;2.35543;-1.35481;,
 -11.25822;1.65502;-2.86226;,
 -11.25822;2.86707;-1.65020;,
 -11.25822;-0.00068;-3.30589;,
 -11.25822;-1.65638;-2.86226;,
 -11.25822;-2.86844;-1.65020;,
 -11.25822;-3.31208;0.00551;,
 -11.25822;-2.86844;1.66119;,
 -11.25822;-1.65638;2.87325;,
 -11.25822;-0.00068;3.31690;,
 -11.25822;1.65502;2.87325;,
 -11.25822;2.86707;1.66119;,
 -11.25822;3.31071;0.00551;,
 -11.25822;2.86707;-1.65020;,
 -12.37305;1.71942;-2.97379;,
 -12.37305;2.97862;-1.71460;,
 -12.37305;-0.00068;-3.43469;,
 -12.37305;-1.72078;-2.97379;,
 -12.37305;-2.97998;-1.71460;,
 -12.37305;-3.44088;0.00551;,
 -12.37305;-2.97998;1.72561;,
 -12.37305;-1.72078;2.98480;,
 -12.37305;-0.00068;3.44571;,
 -12.37305;1.71942;2.98480;,
 -12.37305;2.97862;1.72561;,
 -12.37305;3.43952;0.00551;,
 -12.37305;2.97862;-1.71460;,
 -13.69837;2.38279;-1.37060;,
 -13.69837;1.37541;-2.37798;,
 -13.69837;1.03138;-1.78208;,
 -13.69837;1.78690;-1.02656;,
 -13.69837;-0.00068;-2.74670;,
 -13.69837;-0.00068;-2.05862;,
 -13.69837;-1.37678;-2.37798;,
 -13.69837;-1.03274;-1.78208;,
 -13.69837;-2.38415;-1.37060;,
 -13.69837;-1.78826;-1.02656;,
 -13.69837;-2.75288;0.00551;,
 -13.69837;-2.06480;0.00551;,
 -13.69837;-2.38415;1.38159;,
 -13.69837;-1.78826;1.03756;,
 -13.69837;-1.37678;2.38898;,
 -13.69837;-1.03274;1.79309;,
 -13.69837;-0.00068;2.75769;,
 -13.69837;-0.00068;2.06963;,
 -13.69837;1.37541;2.38898;,
 -13.69837;1.03138;1.79309;,
 -13.69837;2.38279;1.38159;,
 -13.69837;1.78690;1.03756;,
 -13.69837;2.75151;0.00551;,
 -13.69837;2.06344;0.00551;,
 -13.69837;0.68736;-1.18623;,
 -13.69837;1.19104;-0.68253;,
 -13.69837;-0.00068;-1.37058;,
 -13.69837;-0.68872;-1.18623;,
 -13.69837;-1.19240;-0.68253;,
 -13.69837;-1.37676;0.00551;,
 -13.69837;-1.19240;0.69355;,
 -13.69837;-0.68872;1.19722;,
 -13.69837;-0.00068;1.38158;,
 -13.69837;0.68736;1.19722;,
 -13.69837;1.19104;0.69355;,
 -13.69837;1.37540;0.00551;,
 -13.69837;0.34334;-0.59035;,
 -13.69837;0.59518;-0.33851;,
 -13.69837;-0.00068;-0.68253;,
 -13.69837;-0.34470;-0.59035;,
 -13.69837;-0.59654;-0.33851;,
 -13.69837;-0.68872;0.00551;,
 -13.69837;-0.59654;0.34953;,
 -13.69837;-0.34470;0.60137;,
 -13.69837;-0.00068;0.69355;,
 -13.69837;0.34334;0.60137;,
 -13.69837;0.59518;0.34953;,
 -13.69837;0.68736;0.00551;,
 -0.04023;-0.00068;0.00551;,
 -13.69837;-0.00068;0.00551;,
 -13.05837;1.71942;-2.97379;,
 -13.05837;2.97862;-1.71460;,
 -13.05837;-0.00068;-3.43469;,
 -13.05837;-1.72078;-2.97379;,
 -13.05837;-2.97998;-1.71460;,
 -13.05837;-3.44088;0.00551;,
 -13.05837;-2.97998;1.72561;,
 -13.05837;-1.72078;2.98480;,
 -13.05837;-0.00068;3.44571;,
 -13.05837;1.71942;2.98480;,
 -13.05837;2.97862;1.72561;,
 -13.05837;3.43952;0.00551;,
 -13.05837;2.97862;-1.71460;,
 -13.55407;2.62620;-1.51113;,
 -13.55407;3.03258;0.00551;,
 -13.31980;2.84428;-1.63704;,
 -13.31980;3.28439;0.00551;,
 -13.55407;2.62620;-1.51113;,
 -13.55407;3.03258;0.00551;,
 -13.55407;1.51595;-2.62139;,
 -13.55407;2.62620;-1.51113;,
 -13.31980;2.84428;-1.63704;,
 -13.31980;1.64186;-2.83946;,
 -13.55407;1.51595;-2.62139;,
 -13.55407;-0.00068;-3.02776;,
 -13.31980;-0.00068;-3.27957;,
 -13.55407;-0.00068;-3.02776;,
 -13.55407;-1.51731;-2.62139;,
 -13.31980;-1.64322;-2.83946;,
 -13.55407;-1.51731;-2.62139;,
 -13.55407;-2.62757;-1.51113;,
 -13.31980;-2.84564;-1.63704;,
 -13.55407;-2.62757;-1.51113;,
 -13.55407;-3.03394;0.00551;,
 -13.31980;-3.28576;0.00551;,
 -13.55407;-3.03394;0.00551;,
 -13.55407;-2.62757;1.52213;,
 -13.31980;-2.84564;1.64804;,
 -13.55407;-2.62757;1.52213;,
 -13.55407;-1.51731;2.63239;,
 -13.31980;-1.64322;2.85047;,
 -13.55407;-1.51731;2.63239;,
 -13.55407;-0.00068;3.03876;,
 -13.31980;-0.00068;3.29058;,
 -13.55407;-0.00068;3.03876;,
 -13.55407;1.51595;2.63239;,
 -13.31980;1.64186;2.85047;,
 -13.55407;1.51595;2.63239;,
 -13.55407;2.62620;1.52213;,
 -13.31980;2.84428;1.64804;,
 -13.55407;2.62620;1.52213;,
 -0.62174;2.26864;-1.30470;,
 -0.62174;1.30951;-2.26383;,
 -0.62174;-0.00068;-2.61489;,
 -0.62174;-1.31088;-2.26383;,
 -0.62174;-2.27001;-1.30470;,
 -0.62174;-2.62108;0.00551;,
 -0.62174;-2.27001;1.31569;,
 -0.62174;-1.31088;2.27482;,
 -0.62174;-0.00068;2.62589;,
 -0.62174;1.30951;2.27482;,
 -0.62174;2.26864;1.31569;,
 -0.62174;2.61971;0.00551;,
 -0.62174;2.26864;-1.30470;,
 -0.38420;2.16632;-1.24562;,
 -0.38420;2.50155;0.00551;,
 -0.17135;2.00021;-1.14972;,
 -0.17135;2.30975;0.00551;,
 -0.38420;2.16632;-1.24562;,
 -0.38420;2.50155;0.00551;,
 -0.38420;2.16632;-1.24562;,
 -0.38420;1.25044;-2.16150;,
 -0.17135;1.15453;-1.99538;,
 -0.38420;1.25044;-2.16150;,
 -0.38420;-0.00068;-2.49674;,
 -0.17135;-0.00068;-2.30493;,
 -0.38420;-0.00068;-2.49674;,
 -0.38420;-1.25180;-2.16150;,
 -0.17135;-1.15590;-1.99538;,
 -0.38420;-1.25180;-2.16150;,
 -0.38420;-2.16768;-1.24562;,
 -0.17135;-2.00157;-1.14972;,
 -0.38420;-2.16768;-1.24562;,
 -0.38420;-2.50292;0.00551;,
 -0.17135;-2.31111;0.00551;,
 -0.38420;-2.50292;0.00551;,
 -0.38420;-2.16768;1.25661;,
 -0.17135;-2.00157;1.16071;,
 -0.38420;-2.16768;1.25661;,
 -0.38420;-1.25180;2.17250;,
 -0.17135;-1.15590;2.00639;,
 -0.38420;-1.25180;2.17250;,
 -0.38420;-0.00068;2.50773;,
 -0.17135;-0.00068;2.31592;,
 -0.38420;-0.00068;2.50773;,
 -0.38420;1.25044;2.17250;,
 -0.17135;1.15453;2.00639;,
 -0.38420;1.25044;2.17250;,
 -0.38420;2.16632;1.25661;,
 -0.17135;2.00021;1.16071;,
 -0.38420;2.16632;1.25661;;
 
 252;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;3,2,6,7;,
 4;2,5,8,6;,
 4;7,6,9,10;,
 4;6,8,11,9;,
 4;10,9,12,13;,
 4;9,11,14,12;,
 4;13,12,15,16;,
 4;12,14,17,15;,
 4;16,15,18,19;,
 4;15,17,20,18;,
 4;19,18,21,22;,
 4;18,20,23,21;,
 4;22,21,24,25;,
 4;21,23,26,24;,
 4;27,28,29,30;,
 4;4,27,30,5;,
 4;30,29,31,32;,
 4;5,30,32,8;,
 4;33,34,35,36;,
 4;34,0,3,35;,
 4;36,35,37,38;,
 4;35,3,7,37;,
 4;39,40,41,42;,
 4;40,43,44,41;,
 4;43,45,46,44;,
 4;45,47,48,46;,
 4;47,49,50,48;,
 4;49,51,52,50;,
 4;51,53,54,52;,
 4;53,55,56,54;,
 4;55,57,58,56;,
 4;57,59,60,58;,
 4;59,61,62,60;,
 4;61,39,42,62;,
 4;42,41,63,64;,
 4;41,44,65,63;,
 4;44,46,66,65;,
 4;46,48,67,66;,
 4;48,50,68,67;,
 4;50,52,69,68;,
 4;52,54,70,69;,
 4;54,56,71,70;,
 4;56,58,72,71;,
 4;58,60,73,72;,
 4;60,62,74,73;,
 4;62,42,64,74;,
 4;64,63,75,76;,
 4;63,65,77,75;,
 4;65,66,78,77;,
 4;66,67,79,78;,
 4;67,68,80,79;,
 4;68,69,81,80;,
 4;69,70,82,81;,
 4;70,71,83,82;,
 4;71,72,84,83;,
 4;72,73,85,84;,
 4;73,74,86,85;,
 4;74,64,76,86;,
 4;87,88,89,90;,
 4;88,91,92,89;,
 4;91,93,94,92;,
 4;93,95,96,94;,
 4;95,97,98,96;,
 4;97,99,100,98;,
 4;99,101,102,100;,
 4;101,103,104,102;,
 4;103,105,106,104;,
 4;105,107,108,106;,
 4;107,109,110,108;,
 4;109,111,112,110;,
 4;90,89,113,114;,
 4;89,92,115,113;,
 4;92,94,116,115;,
 4;94,96,117,116;,
 4;96,98,118,117;,
 4;98,100,119,118;,
 4;100,102,120,119;,
 4;102,104,121,120;,
 4;104,106,122,121;,
 4;106,108,123,122;,
 4;108,110,124,123;,
 4;110,112,125,124;,
 4;114,113,126,127;,
 4;113,115,128,126;,
 4;115,116,129,128;,
 4;116,117,130,129;,
 4;117,118,131,130;,
 4;118,119,132,131;,
 4;119,120,133,132;,
 4;120,121,134,133;,
 4;121,122,135,134;,
 4;122,123,136,135;,
 4;123,124,137,136;,
 4;124,125,138,137;,
 4;139,140,141,142;,
 4;140,143,144,141;,
 4;143,145,146,144;,
 4;145,147,148,146;,
 4;147,149,150,148;,
 4;149,151,152,150;,
 4;151,153,154,152;,
 4;153,155,156,154;,
 4;155,157,158,156;,
 4;157,159,160,158;,
 4;159,161,162,160;,
 4;161,139,142,162;,
 4;142,141,163,164;,
 4;141,144,165,163;,
 4;144,146,166,165;,
 4;146,148,167,166;,
 4;148,150,168,167;,
 4;150,152,169,168;,
 4;152,154,170,169;,
 4;154,156,171,170;,
 4;156,158,172,171;,
 4;158,160,173,172;,
 4;160,162,174,173;,
 4;162,142,164,174;,
 4;164,163,175,176;,
 4;163,165,177,175;,
 4;165,166,178,177;,
 4;166,167,179,178;,
 4;167,168,180,179;,
 4;168,169,181,180;,
 4;169,170,182,181;,
 4;170,171,183,182;,
 4;171,172,184,183;,
 4;172,173,185,184;,
 4;173,174,186,185;,
 4;174,164,176,186;,
 3;40,39,187;,
 3;43,40,187;,
 3;45,43,187;,
 3;47,45,187;,
 3;49,47,187;,
 3;51,49,187;,
 3;53,51,187;,
 3;55,53,187;,
 3;57,55,187;,
 3;59,57,187;,
 3;61,59,187;,
 3;39,61,187;,
 3;176,175,188;,
 3;175,177,188;,
 3;177,178,188;,
 3;178,179,188;,
 3;179,180,188;,
 3;180,181,188;,
 3;181,182,188;,
 3;182,183,188;,
 3;183,184,188;,
 3;184,185,188;,
 3;185,186,188;,
 3;186,176,188;,
 4;127,126,189,190;,
 4;126,128,191,189;,
 4;128,129,192,191;,
 4;129,130,193,192;,
 4;130,131,194,193;,
 4;131,132,195,194;,
 4;132,133,196,195;,
 4;133,134,197,196;,
 4;134,135,198,197;,
 4;135,136,199,198;,
 4;136,137,200,199;,
 4;137,138,201,200;,
 4;202,139,161,203;,
 4;201,204,205,200;,
 4;204,206,207,205;,
 4;139,202,208,140;,
 4;209,210,211,212;,
 4;210,190,189,211;,
 4;140,208,213,143;,
 4;212,211,214,215;,
 4;211,189,191,214;,
 4;143,213,216,145;,
 4;215,214,217,218;,
 4;214,191,192,217;,
 4;145,216,219,147;,
 4;218,217,220,221;,
 4;217,192,193,220;,
 4;147,219,222,149;,
 4;221,220,223,224;,
 4;220,193,194,223;,
 4;149,222,225,151;,
 4;224,223,226,227;,
 4;223,194,195,226;,
 4;151,225,228,153;,
 4;227,226,229,230;,
 4;226,195,196,229;,
 4;153,228,231,155;,
 4;230,229,232,233;,
 4;229,196,197,232;,
 4;155,231,234,157;,
 4;233,232,235,236;,
 4;232,197,198,235;,
 4;157,234,237,159;,
 4;236,235,238,239;,
 4;235,198,199,238;,
 4;159,237,203,161;,
 4;239,238,205,207;,
 4;238,199,200,205;,
 4;240,241,88,87;,
 4;241,242,91,88;,
 4;242,243,93,91;,
 4;243,244,95,93;,
 4;244,245,97,95;,
 4;245,246,99,97;,
 4;246,247,101,99;,
 4;247,248,103,101;,
 4;248,249,105,103;,
 4;249,250,107,105;,
 4;250,251,109,107;,
 4;251,252,111,109;,
 4;253,252,251,254;,
 4;76,255,256,86;,
 4;255,257,258,256;,
 4;240,259,260,241;,
 4;257,255,261,262;,
 4;255,76,75,261;,
 4;241,260,263,242;,
 4;262,261,264,265;,
 4;261,75,77,264;,
 4;242,263,266,243;,
 4;265,264,267,268;,
 4;264,77,78,267;,
 4;243,266,269,244;,
 4;268,267,270,271;,
 4;267,78,79,270;,
 4;244,269,272,245;,
 4;271,270,273,274;,
 4;270,79,80,273;,
 4;245,272,275,246;,
 4;274,273,276,277;,
 4;273,80,81,276;,
 4;246,275,278,247;,
 4;277,276,279,280;,
 4;276,81,82,279;,
 4;247,278,281,248;,
 4;280,279,282,283;,
 4;279,82,83,282;,
 4;248,281,284,249;,
 4;283,282,285,286;,
 4;282,83,84,285;,
 4;249,284,287,250;,
 4;286,285,288,289;,
 4;285,84,85,288;,
 4;250,287,254,251;,
 4;289,288,256,258;,
 4;288,85,86,256;;
 
 MeshMaterialList {
  2;
  252;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.851000;0.043000;0.043000;1.000000;;
   33.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;0.992157;0.729412;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\white2.jpg";
   }
  }
 }
 MeshNormals {
  184;
  0.000000;0.000000;1.000000;,
  -0.995546;0.094282;-0.000001;,
  -0.995545;0.094285;-0.000001;,
  -0.995545;0.094288;-0.000001;,
  -0.879357;0.083279;-0.468824;,
  -0.879357;0.083281;-0.468823;,
  -0.879357;0.083283;-0.468823;,
  0.000000;0.000000;-1.000000;,
  0.995546;-0.094282;-0.000001;,
  0.995546;-0.094282;-0.000001;,
  0.995546;-0.094282;-0.000001;,
  0.094283;0.995545;0.000000;,
  -0.094281;-0.995546;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.962860;0.233829;-0.135002;,
  0.962858;0.135003;-0.233833;,
  0.962859;0.000000;-0.270006;,
  0.962859;-0.135003;-0.233832;,
  0.962859;-0.233831;-0.135002;,
  0.962858;-0.270007;0.000001;,
  0.962858;-0.233834;0.135004;,
  0.962858;-0.135004;0.233835;,
  0.962857;0.000000;0.270012;,
  0.962857;0.135004;0.233836;,
  0.962859;0.233831;0.135004;,
  0.962860;0.270001;0.000001;,
  0.034276;0.865517;-0.499706;,
  0.034277;0.499705;-0.865517;,
  0.034277;0.000001;-0.999412;,
  0.034275;-0.499704;-0.865517;,
  0.034274;-0.865516;-0.499707;,
  0.034274;-0.999412;0.000003;,
  0.034274;-0.865515;0.499709;,
  0.034275;-0.499708;0.865516;,
  0.034277;0.000001;0.999412;,
  0.034277;0.499708;0.865515;,
  0.034276;0.865515;0.499708;,
  0.034275;0.999412;0.000003;,
  0.068703;0.863980;-0.498817;,
  0.068703;0.498818;-0.863980;,
  0.068703;0.000000;-0.997637;,
  0.068702;-0.498817;-0.863980;,
  0.068701;-0.863979;-0.498819;,
  0.068700;-0.997637;0.000003;,
  0.068701;-0.863978;0.498821;,
  0.068702;-0.498821;0.863978;,
  0.068703;0.000000;0.997637;,
  0.068703;0.498821;0.863978;,
  0.068703;0.863979;0.498819;,
  0.068702;0.997637;0.000003;,
  0.091863;0.862365;-0.497884;,
  0.091860;0.497885;-0.862365;,
  0.091858;0.000000;-0.995772;,
  0.091859;-0.497884;-0.862365;,
  0.091860;-0.862364;-0.497886;,
  0.091862;-0.995772;0.000002;,
  0.091864;-0.862363;0.497887;,
  0.091866;-0.497887;0.862362;,
  0.091866;0.000000;0.995771;,
  0.091866;0.497888;0.862362;,
  0.091867;0.862363;0.497886;,
  0.091866;0.995771;0.000002;,
  0.057494;0.864593;-0.499173;,
  0.057490;0.499171;-0.864594;,
  0.057489;0.000000;-0.998346;,
  0.057490;-0.499171;-0.864594;,
  0.057491;-0.864593;-0.499174;,
  0.057492;-0.998346;0.000001;,
  0.057495;-0.864592;0.499174;,
  0.057496;-0.499174;0.864592;,
  0.057496;0.000000;0.998346;,
  0.057497;0.499174;0.864592;,
  0.057498;0.864592;0.499174;,
  0.057497;0.998346;0.000001;,
  -0.972221;0.202708;-0.117033;,
  -0.972220;0.117033;-0.202709;,
  -0.972220;-0.000000;-0.234069;,
  -0.972220;-0.117033;-0.202709;,
  -0.972221;-0.202708;-0.117033;,
  -0.972221;-0.234066;0.000000;,
  -0.972221;-0.202706;0.117032;,
  -0.972221;-0.117032;0.202706;,
  -0.972221;-0.000000;0.234066;,
  -0.972221;0.117032;0.202706;,
  -0.972221;0.202706;0.117032;,
  -0.972221;0.234066;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.265737;0.834887;-0.482024;,
  -0.628157;0.673843;-0.389043;,
  -0.818961;0.496968;-0.286923;,
  -0.265730;0.482022;-0.834891;,
  -0.628149;0.389045;-0.673849;,
  -0.818960;0.286924;-0.496970;,
  -0.265728;-0.000000;-0.964048;,
  -0.628146;-0.000000;-0.778096;,
  -0.818956;-0.000000;-0.573856;,
  -0.265730;-0.482022;-0.834891;,
  -0.628148;-0.389045;-0.673850;,
  -0.818958;-0.286925;-0.496972;,
  -0.265733;-0.834888;-0.482024;,
  -0.628153;-0.673846;-0.389045;,
  -0.818961;-0.496969;-0.286924;,
  -0.265736;-0.964046;0.000001;,
  -0.628156;-0.778088;0.000001;,
  -0.818962;-0.573848;0.000001;,
  -0.265736;-0.834887;0.482024;,
  -0.628156;-0.673843;0.389044;,
  -0.818962;-0.496967;0.286923;,
  -0.265736;-0.482024;0.834888;,
  -0.628158;-0.389043;0.673842;,
  -0.818964;-0.286922;0.496965;,
  -0.265736;-0.000000;0.964046;,
  -0.628159;-0.000000;0.778085;,
  -0.818964;-0.000000;0.573845;,
  -0.265736;0.482023;0.834888;,
  -0.628160;0.389042;0.673841;,
  -0.818965;0.286921;0.496963;,
  -0.265740;0.834887;0.482024;,
  -0.628161;0.673841;0.389041;,
  -0.818964;0.496966;0.286921;,
  -0.265743;0.964044;0.000001;,
  -0.628161;0.778083;0.000001;,
  -0.818962;0.573848;0.000001;,
  0.769504;0.553080;-0.319322;,
  0.563068;0.715693;-0.413204;,
  0.229507;0.842909;-0.486653;,
  0.769505;0.319320;-0.553079;,
  0.563072;0.413204;-0.715690;,
  0.229506;0.486653;-0.842909;,
  0.769506;0.000000;-0.638639;,
  0.563075;0.000000;-0.826406;,
  0.229508;0.000001;-0.973307;,
  0.769504;-0.319322;-0.553080;,
  0.563076;-0.413203;-0.715688;,
  0.229513;-0.486651;-0.842908;,
  0.769502;-0.553082;-0.319322;,
  0.563077;-0.715687;-0.413203;,
  0.229517;-0.842906;-0.486652;,
  0.769501;-0.638646;0.000002;,
  0.563077;-0.826405;0.000002;,
  0.229518;-0.973304;0.000003;,
  0.769498;-0.553086;0.319324;,
  0.563071;-0.715690;0.413205;,
  0.229514;-0.842906;0.486655;,
  0.769498;-0.319324;0.553087;,
  0.563069;-0.413205;0.715692;,
  0.229510;-0.486654;0.842908;,
  0.769498;0.000000;0.638650;,
  0.563069;0.000000;0.826410;,
  0.229508;0.000001;0.973307;,
  0.769500;0.319323;0.553085;,
  0.563065;0.413207;0.715694;,
  0.229503;0.486656;0.842908;,
  0.769500;0.553085;0.319324;,
  0.563062;0.715696;0.413207;,
  0.229504;0.842908;0.486656;,
  0.769500;0.638646;0.000002;,
  0.563064;0.826413;0.000002;,
  0.229509;0.973307;0.000003;,
  -0.879358;0.083279;0.468823;,
  -0.879358;0.083281;0.468822;,
  -0.879358;0.083284;0.468821;,
  0.879357;-0.083279;-0.468824;,
  0.879357;-0.083279;-0.468824;,
  0.879357;-0.083278;-0.468824;,
  0.879358;-0.083279;0.468823;,
  0.879358;-0.083279;0.468823;,
  0.879358;-0.083278;0.468823;,
  0.094282;0.995546;0.000000;,
  0.094282;0.995546;-0.000001;,
  0.094283;0.995546;-0.000001;,
  0.094282;0.995546;0.000002;,
  0.094283;0.995546;0.000001;,
  0.094284;0.995545;-0.000000;,
  0.094284;0.995545;-0.000000;,
  0.094284;0.995545;0.000000;,
  -0.094279;-0.995546;-0.000002;,
  -0.094279;-0.995546;-0.000000;,
  -0.094281;-0.995546;-0.000001;,
  -0.094279;-0.995546;0.000001;,
  -0.094281;-0.995546;0.000000;,
  -0.094283;-0.995545;-0.000000;,
  -0.094283;-0.995545;0.000000;,
  -0.094283;-0.995545;-0.000000;;
  252;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;159,160,2,1;,
  4;160,161,3,2;,
  4;1,2,5,4;,
  4;2,3,6,5;,
  4;7,7,7,7;,
  4;7,7,7,7;,
  4;7,7,7,7;,
  4;7,7,7,7;,
  4;162,163,9,8;,
  4;163,164,10,9;,
  4;8,9,165,166;,
  4;9,10,167,165;,
  4;168,169,170,11;,
  4;171,168,11,172;,
  4;11,170,173,174;,
  4;172,11,174,175;,
  4;176,177,12,178;,
  4;177,179,180,12;,
  4;178,12,181,182;,
  4;12,180,183,181;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,15,14;,
  4;13,13,16,15;,
  4;13,13,17,16;,
  4;13,13,18,17;,
  4;13,13,19,18;,
  4;13,13,20,19;,
  4;13,13,21,20;,
  4;13,13,22,21;,
  4;13,13,23,22;,
  4;13,13,24,23;,
  4;13,13,25,24;,
  4;13,13,14,25;,
  4;26,27,39,38;,
  4;27,28,40,39;,
  4;28,29,41,40;,
  4;29,30,42,41;,
  4;30,31,43,42;,
  4;31,32,44,43;,
  4;32,33,45,44;,
  4;33,34,46,45;,
  4;34,35,47,46;,
  4;35,36,48,47;,
  4;36,37,49,48;,
  4;37,26,38,49;,
  4;38,39,51,50;,
  4;39,40,52,51;,
  4;40,41,53,52;,
  4;41,42,54,53;,
  4;42,43,55,54;,
  4;43,44,56,55;,
  4;44,45,57,56;,
  4;45,46,58,57;,
  4;46,47,59,58;,
  4;47,48,60,59;,
  4;48,49,61,60;,
  4;49,38,50,61;,
  4;50,51,63,62;,
  4;51,52,64,63;,
  4;52,53,65,64;,
  4;53,54,66,65;,
  4;54,55,67,66;,
  4;55,56,68,67;,
  4;56,57,69,68;,
  4;57,58,70,69;,
  4;58,59,71,70;,
  4;59,60,72,71;,
  4;60,61,73,72;,
  4;61,50,62,73;,
  4;74,75,86,86;,
  4;75,76,86,86;,
  4;76,77,86,86;,
  4;77,78,86,86;,
  4;78,79,86,86;,
  4;79,80,86,86;,
  4;80,81,86,86;,
  4;81,82,86,86;,
  4;82,83,86,86;,
  4;83,84,86,86;,
  4;84,85,86,86;,
  4;85,74,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;13,13,13;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  3;86,86,86;,
  4;62,63,90,87;,
  4;63,64,93,90;,
  4;64,65,96,93;,
  4;65,66,99,96;,
  4;66,67,102,99;,
  4;67,68,105,102;,
  4;68,69,108,105;,
  4;69,70,111,108;,
  4;70,71,114,111;,
  4;71,72,117,114;,
  4;72,73,120,117;,
  4;73,62,87,120;,
  4;89,74,85,122;,
  4;87,88,121,120;,
  4;88,89,122,121;,
  4;74,89,92,75;,
  4;89,88,91,92;,
  4;88,87,90,91;,
  4;75,92,95,76;,
  4;92,91,94,95;,
  4;91,90,93,94;,
  4;76,95,98,77;,
  4;95,94,97,98;,
  4;94,93,96,97;,
  4;77,98,101,78;,
  4;98,97,100,101;,
  4;97,96,99,100;,
  4;78,101,104,79;,
  4;101,100,103,104;,
  4;100,99,102,103;,
  4;79,104,107,80;,
  4;104,103,106,107;,
  4;103,102,105,106;,
  4;80,107,110,81;,
  4;107,106,109,110;,
  4;106,105,108,109;,
  4;81,110,113,82;,
  4;110,109,112,113;,
  4;109,108,111,112;,
  4;82,113,116,83;,
  4;113,112,115,116;,
  4;112,111,114,115;,
  4;83,116,119,84;,
  4;116,115,118,119;,
  4;115,114,117,118;,
  4;84,119,122,85;,
  4;119,118,121,122;,
  4;118,117,120,121;,
  4;125,128,27,26;,
  4;128,131,28,27;,
  4;131,134,29,28;,
  4;134,137,30,29;,
  4;137,140,31,30;,
  4;140,143,32,31;,
  4;143,146,33,32;,
  4;146,149,34,33;,
  4;149,152,35,34;,
  4;152,155,36,35;,
  4;155,158,37,36;,
  4;158,125,26,37;,
  4;124,125,158,157;,
  4;14,123,156,25;,
  4;123,124,157,156;,
  4;125,124,127,128;,
  4;124,123,126,127;,
  4;123,14,15,126;,
  4;128,127,130,131;,
  4;127,126,129,130;,
  4;126,15,16,129;,
  4;131,130,133,134;,
  4;130,129,132,133;,
  4;129,16,17,132;,
  4;134,133,136,137;,
  4;133,132,135,136;,
  4;132,17,18,135;,
  4;137,136,139,140;,
  4;136,135,138,139;,
  4;135,18,19,138;,
  4;140,139,142,143;,
  4;139,138,141,142;,
  4;138,19,20,141;,
  4;143,142,145,146;,
  4;142,141,144,145;,
  4;141,20,21,144;,
  4;146,145,148,149;,
  4;145,144,147,148;,
  4;144,21,22,147;,
  4;149,148,151,152;,
  4;148,147,150,151;,
  4;147,22,23,150;,
  4;152,151,154,155;,
  4;151,150,153,154;,
  4;150,23,24,153;,
  4;155,154,157,158;,
  4;154,153,156,157;,
  4;153,24,25,156;;
 }
 MeshTextureCoords {
  290;
  0.375000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.125000;,
  0.375000;0.125000;,
  0.625000;0.000000;,
  0.625000;0.125000;,
  0.500000;0.250000;,
  0.375000;0.250000;,
  0.625000;0.250000;,
  0.500000;0.375000;,
  0.375000;0.375000;,
  0.625000;0.375000;,
  0.500000;0.500000;,
  0.375000;0.500000;,
  0.625000;0.500000;,
  0.500000;0.625000;,
  0.375000;0.625000;,
  0.625000;0.625000;,
  0.500000;0.750000;,
  0.375000;0.750000;,
  0.625000;0.750000;,
  0.500000;0.875000;,
  0.375000;0.875000;,
  0.625000;0.875000;,
  0.500000;1.000000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.875000;0.000000;,
  0.875000;0.125000;,
  0.750000;0.125000;,
  0.875000;0.250000;,
  0.750000;0.250000;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.125000;,
  0.125000;0.125000;,
  0.250000;0.250000;,
  0.125000;0.250000;,
  0.527060;0.140620;,
  0.515620;0.129190;,
  0.531250;0.102120;,
  0.554130;0.125000;,
  0.500000;0.125000;,
  0.500000;0.093750;,
  0.484380;0.129190;,
  0.468750;0.102120;,
  0.472940;0.140620;,
  0.445870;0.125000;,
  0.468750;0.156250;,
  0.437500;0.156250;,
  0.472940;0.171880;,
  0.445870;0.187500;,
  0.484380;0.183310;,
  0.468750;0.210380;,
  0.500000;0.187500;,
  0.500000;0.218750;,
  0.515620;0.183310;,
  0.531250;0.210380;,
  0.527060;0.171880;,
  0.554130;0.187500;,
  0.531250;0.156250;,
  0.562500;0.156250;,
  0.546880;0.075060;,
  0.581190;0.109380;,
  0.500000;0.062500;,
  0.453120;0.075060;,
  0.418810;0.109380;,
  0.406250;0.156250;,
  0.418810;0.203120;,
  0.453120;0.237440;,
  0.500000;0.250000;,
  0.546880;0.237440;,
  0.581190;0.203120;,
  0.593750;0.156250;,
  0.562500;0.048000;,
  0.608260;0.093750;,
  0.500000;0.031250;,
  0.437500;0.048000;,
  0.391750;0.093750;,
  0.375000;0.156250;,
  0.391750;0.218750;,
  0.437500;0.264500;,
  0.500000;0.281250;,
  0.562500;0.264500;,
  0.608260;0.218750;,
  0.625000;0.156250;,
  0.375000;0.387690;,
  0.395830;0.387690;,
  0.395830;0.462880;,
  0.375000;0.462880;,
  0.416670;0.387690;,
  0.416670;0.462880;,
  0.437500;0.387690;,
  0.437500;0.462880;,
  0.458330;0.387690;,
  0.458330;0.462880;,
  0.479170;0.387690;,
  0.479170;0.462880;,
  0.500000;0.387690;,
  0.500000;0.462880;,
  0.520830;0.387690;,
  0.520830;0.462880;,
  0.541670;0.387690;,
  0.541670;0.462880;,
  0.562500;0.387690;,
  0.562500;0.462880;,
  0.583330;0.387690;,
  0.583330;0.462880;,
  0.604170;0.387690;,
  0.604170;0.462880;,
  0.625000;0.387690;,
  0.625000;0.462880;,
  0.395830;0.538060;,
  0.375000;0.538060;,
  0.416670;0.538060;,
  0.437500;0.538060;,
  0.458330;0.538060;,
  0.479170;0.538060;,
  0.500000;0.538060;,
  0.520830;0.538060;,
  0.541670;0.538060;,
  0.562500;0.538060;,
  0.583330;0.538060;,
  0.604170;0.538060;,
  0.625000;0.538060;,
  0.395830;0.613250;,
  0.375000;0.613250;,
  0.416670;0.613250;,
  0.437500;0.613250;,
  0.458330;0.613250;,
  0.479170;0.613250;,
  0.500000;0.613250;,
  0.520830;0.613250;,
  0.541670;0.613250;,
  0.562500;0.613250;,
  0.583330;0.613250;,
  0.604170;0.613250;,
  0.625000;0.613250;,
  0.608260;0.906250;,
  0.562500;0.952010;,
  0.546880;0.924940;,
  0.581190;0.890620;,
  0.500000;0.968750;,
  0.500000;0.937500;,
  0.437500;0.952010;,
  0.453120;0.924940;,
  0.391750;0.906250;,
  0.418810;0.890620;,
  0.375000;0.843750;,
  0.406250;0.843750;,
  0.391750;0.781250;,
  0.418810;0.796880;,
  0.437500;0.735490;,
  0.453120;0.762560;,
  0.500000;0.718750;,
  0.500000;0.750000;,
  0.562500;0.735490;,
  0.546880;0.762560;,
  0.608260;0.781250;,
  0.581190;0.796880;,
  0.625000;0.843750;,
  0.593750;0.843750;,
  0.531250;0.897880;,
  0.554130;0.875000;,
  0.500000;0.906250;,
  0.468750;0.897880;,
  0.445870;0.875000;,
  0.437500;0.843750;,
  0.445870;0.812500;,
  0.468750;0.789620;,
  0.500000;0.781250;,
  0.531250;0.789620;,
  0.554130;0.812500;,
  0.562500;0.843750;,
  0.515620;0.870810;,
  0.527060;0.859380;,
  0.500000;0.875000;,
  0.484380;0.870810;,
  0.472940;0.859380;,
  0.468750;0.843750;,
  0.472940;0.828120;,
  0.484380;0.816690;,
  0.500000;0.812500;,
  0.515620;0.816690;,
  0.527060;0.828120;,
  0.531250;0.843750;,
  0.500000;0.150000;,
  0.500000;0.837500;,
  0.395830;0.652130;,
  0.375000;0.652130;,
  0.416670;0.652130;,
  0.437500;0.652130;,
  0.458330;0.652130;,
  0.479170;0.652130;,
  0.500000;0.652130;,
  0.520830;0.652130;,
  0.541670;0.652130;,
  0.562500;0.652130;,
  0.583330;0.652130;,
  0.604170;0.652130;,
  0.625000;0.652130;,
  0.635320;0.921880;,
  0.656250;0.843750;,
  0.625000;0.669260;,
  0.604170;0.668790;,
  0.625000;0.688440;,
  0.604170;0.688440;,
  0.578120;0.979070;,
  0.375000;0.688440;,
  0.375000;0.669260;,
  0.395830;0.668790;,
  0.395830;0.688440;,
  0.500000;1.000000;,
  0.416670;0.668780;,
  0.416670;0.688440;,
  0.421880;0.979070;,
  0.437500;0.668780;,
  0.437500;0.688440;,
  0.364680;0.921880;,
  0.458330;0.668780;,
  0.458330;0.688440;,
  0.343750;0.843750;,
  0.479170;0.668780;,
  0.479170;0.688440;,
  0.364680;0.765620;,
  0.500000;0.668780;,
  0.500000;0.688440;,
  0.421880;0.708430;,
  0.520830;0.668780;,
  0.520830;0.688440;,
  0.500000;0.687500;,
  0.541670;0.668780;,
  0.541670;0.688440;,
  0.578120;0.708430;,
  0.562500;0.668780;,
  0.562500;0.688440;,
  0.635320;0.765620;,
  0.583330;0.668780;,
  0.583330;0.688440;,
  0.375000;0.348810;,
  0.395830;0.348810;,
  0.416670;0.348810;,
  0.437500;0.348810;,
  0.458330;0.348810;,
  0.479170;0.348810;,
  0.500000;0.348810;,
  0.520830;0.348810;,
  0.541670;0.348810;,
  0.562500;0.348810;,
  0.583330;0.348810;,
  0.604170;0.348810;,
  0.625000;0.348810;,
  0.625000;0.312500;,
  0.604170;0.312500;,
  0.621020;0.086380;,
  0.639740;0.156250;,
  0.635320;0.078120;,
  0.656250;0.156250;,
  0.375000;0.312500;,
  0.395830;0.312500;,
  0.569870;0.035230;,
  0.578120;0.020930;,
  0.416670;0.312500;,
  0.500000;0.016510;,
  0.500000;0.000000;,
  0.437500;0.312500;,
  0.430130;0.035230;,
  0.421880;0.020930;,
  0.458330;0.312500;,
  0.378980;0.086380;,
  0.364680;0.078120;,
  0.479170;0.312500;,
  0.360260;0.156250;,
  0.343750;0.156250;,
  0.500000;0.312500;,
  0.378980;0.226120;,
  0.364680;0.234380;,
  0.520830;0.312500;,
  0.430130;0.277270;,
  0.421880;0.291570;,
  0.541670;0.312500;,
  0.500000;0.295990;,
  0.500000;0.312500;,
  0.562500;0.312500;,
  0.569870;0.277270;,
  0.578120;0.291570;,
  0.583330;0.312500;,
  0.621020;0.226120;,
  0.635320;0.234380;;
 }
}
