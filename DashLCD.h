#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "FTDI _V1/FT_VM801P43_50.h"

#include <AVRLibrary/CPFECANLib.h>
#include "float16.hpp"
#include <AVRLibrary/arduino/Arduino.h>
#include "CPFERotarySwitch.h"

static const uint8_t PROGMEM CPRacingLogo[] = {120, 156, 237, 91, 45, 116, 234,
	74, 16, 174, 172, 68, 98, 43, 145, 72, 44, 18, 89, 137, 173, 68, 86, 98,
	174, 64, 32, 16, 136, 8, 4, 166, 34, 2, 129, 168, 64, 84, 196, 32, 34,
	42, 16, 8, 4, 34, 6, 17, 129, 136, 168, 136, 64, 244, 157, 57, 115, 230,
	124, 51, 155, 77, 128, 94, 222, 123, 183, 183, 217, 17, 109, 55, 155,
	221, 205, 252, 124, 243, 179, 219, 187, 95, 119, 53, 213, 84, 83, 77,
	53, 213, 84, 83, 77, 53, 213, 84, 83, 77, 53, 253, 161, 52, 88, 206, 63,
	254, 14, 122, 24, 221, 253, 26, 77, 55, 111, 85, 52, 121, 29, 77, 171,
	184, 209, 91, 232, 209, 85, 99, 239, 95, 202, 214, 192, 91, 118, 55,
	247, 47, 190, 94, 222, 149, 157, 155, 123, 117, 79, 119, 60, 121, 173,
	250, 174, 234, 175, 114, 169, 177, 254, 252, 43, 90, 148, 179, 36, 206,
	143, 235, 45, 202, 120, 49, 154, 218, 177, 150, 239, 154, 158, 55, 229,
	43, 196, 51, 232, 133, 111, 54, 223, 30, 187, 99, 61, 59, 247, 225, 239,
	120, 22, 229, 213, 95, 101, 223, 63, 71, 171, 221, 57, 46, 125, 143,
	214, 73, 217, 66, 206, 143, 28, 102, 151, 74, 50, 79, 252, 227, 170, 87,
	41, 27, 39, 50, 247, 189, 173, 45, 61, 158, 217, 93, 118, 199, 231, 36,
	174, 215, 188, 132, 58, 233, 121, 46, 125, 135, 182, 218, 21, 249, 217,
	73, 243, 132, 201, 142, 245, 115, 194, 103, 127, 190, 113, 174, 12, 100,
	13, 161, 50, 153, 11, 182, 115, 175, 157, 131, 119, 175, 247, 129, 121,
	236, 44, 195, 204, 93, 47, 79, 158, 55, 215, 201, 156, 215, 110, 30, 92,
	143, 48, 121, 197, 58, 205, 131, 244, 102, 145, 143, 35, 143, 239, 85,
	114, 176, 244, 48, 226, 153, 231, 31, 194, 3, 205, 77, 158, 105, 176,
	44, 74, 1, 60, 8, 3, 244, 46, 87, 119, 191, 150, 43, 250, 173, 29, 211,
	51, 139, 90, 88, 193, 106, 182, 159, 19, 174, 102, 216, 25, 252, 156,
	169, 194, 127, 119, 70, 193, 124, 222, 163, 139, 41, 64, 103, 150, 49,
	228, 56, 204, 48, 170, 28, 163, 46, 167, 135, 17, 205, 20, 229, 178, 98,
	119, 60, 154, 50, 105, 57, 194, 67, 165, 115, 244, 206, 63, 164, 119,
	183, 47, 114, 11, 114, 32, 162, 25, 241, 85, 196, 139, 116, 174, 159,
	81, 252, 5, 29, 148, 103, 150, 187, 216, 133, 72, 150, 244, 96, 52, 77,
	78, 244, 59, 73, 223, 229, 50, 86, 212, 189, 236, 5, 252, 146, 204, 19,
	171, 29, 62, 137, 250, 173, 247, 188, 204, 109, 175, 139, 42, 64, 119,
	158, 31, 43, 235, 81, 101, 222, 230, 26, 98, 155, 121, 124, 23, 137, 51,
	255, 108, 179, 200, 129, 17, 224, 93, 255, 88, 124, 11, 114, 16, 123,
	213, 58, 130, 223, 153, 3, 253, 163, 252, 221, 60, 124, 126, 62, 77,
	248, 119, 27, 95, 2, 53, 216, 227, 69, 57, 141, 19, 25, 181, 66, 122,
	162, 109, 2, 92, 238, 142, 207, 241, 173, 59, 230, 55, 41, 26, 63, 55,
	214, 98, 9, 52, 177, 72, 122, 28, 199, 152, 216, 163, 59, 15, 184, 201,
	253, 162, 3, 252, 173, 210, 96, 69, 95, 167, 86, 168, 101, 163, 173, 88,
	90, 114, 122, 154, 16, 230, 66, 94, 69, 25, 248, 52, 5, 114, 16, 27,
	130, 92, 251, 199, 221, 222, 242, 197, 234, 143, 252, 238, 226, 95, 24,
	232, 93, 144, 7, 128, 175, 225, 103, 126, 46, 90, 155, 242, 241, 141,
	37, 77, 18, 118, 237, 175, 56, 214, 111, 189, 231, 100, 14, 237, 161,
	191, 72, 3, 44, 138, 125, 126, 138, 119, 21, 28, 240, 237, 189, 202,
	147, 92, 74, 236, 11, 69, 54, 131, 101, 81, 110, 89, 68, 246, 6, 45,
	133, 29, 8, 239, 172, 21, 233, 38, 54, 192, 146, 129, 156, 147, 147,
	232, 208, 211, 132, 71, 106, 253, 129, 207, 114, 61, 108, 176, 149, 39,
	233, 156, 222, 208, 54, 64, 189, 54, 215, 138, 114, 137, 113, 172, 245,
	23, 249, 38, 81, 25, 239, 215, 70, 87, 197, 204, 78, 63, 7, 95, 138,
	100, 165, 37, 95, 200, 123, 36, 13, 112, 249, 38, 251, 210, 123, 113,
	61, 73, 111, 225, 175, 64, 92, 35, 243, 118, 76, 51, 137, 253, 216, 8,
	141, 26, 85, 58, 158, 38, 100, 199, 178, 7, 240, 89, 208, 193, 162, 143,
	110, 226, 79, 217, 94, 73, 123, 68, 71, 100, 151, 130, 44, 90, 127, 96,
	17, 86, 86, 212, 4, 245, 41, 18, 121, 24, 225, 57, 251, 138, 243, 217,
	57, 243, 205, 229, 2, 239, 79, 124, 152, 197, 220, 34, 42, 232, 167, 85,
	254, 181, 42, 59, 231, 247, 44, 142, 9, 46, 241, 95, 50, 139, 27, 61,
	248, 243, 196, 107, 100, 206, 223, 39, 17, 84, 177, 210, 64, 248, 201,
	189, 208, 37, 209, 115, 65, 135, 222, 194, 207, 219, 228, 4, 249, 116,
	199, 136, 167, 8, 77, 132, 239, 130, 44, 50, 59, 233, 15, 172, 185, 56,
	39, 124, 140, 205, 22, 56, 179, 112, 209, 210, 215, 138, 17, 156, 232,
	172, 96, 171, 157, 197, 29, 111, 53, 220, 151, 153, 248, 165, 37, 95,
	200, 179, 243, 95, 46, 231, 104, 7, 140, 3, 192, 15, 171, 129, 221, 177,
	79, 230, 85, 104, 83, 36, 138, 128, 240, 134, 139, 165, 252, 189, 79,
	19, 27, 117, 203, 46, 4, 29, 202, 42, 20, 240, 223, 44, 115, 162, 167,
	201, 96, 73, 58, 227, 34, 151, 232, 0, 113, 161, 177, 230, 223, 5, 247,
	117, 75, 78, 178, 15, 107, 69, 172, 39, 118, 39, 207, 155, 205, 155,
	253, 162, 60, 217, 188, 21, 43, 86, 60, 102, 152, 9, 78, 150, 103, 81,
	197, 117, 171, 80, 181, 42, 59, 183, 56, 174, 103, 115, 179, 115, 55,
	122, 144, 234, 176, 253, 174, 107, 100, 78, 107, 66, 147, 93, 44, 69,
	62, 165, 137, 99, 0, 160, 67, 89, 118, 238, 198, 105, 62, 62, 202, 187,
	90, 7, 36, 98, 245, 69, 148, 101, 152, 196, 122, 98, 251, 136, 203, 214,
	146, 124, 86, 89, 134, 82, 104, 214, 23, 248, 173, 215, 71, 254, 90, 1,
	239, 209, 254, 37, 141, 36, 193, 56, 128, 157, 106, 173, 40, 139, 74,
	171, 208, 198, 37, 206, 206, 117, 108, 229, 206, 164, 163, 58, 209, 0,
	23, 125, 202, 178, 115, 55, 78, 211, 205, 125, 87, 123, 126, 225, 135,
	47, 162, 44, 195, 36, 214, 147, 98, 68, 237, 198, 73, 69, 43, 47, 198,
	12, 110, 179, 30, 253, 86, 217, 185, 95, 227, 40, 66, 163, 159, 231,
	178, 243, 175, 71, 243, 156, 157, 67, 230, 131, 165, 224, 219, 110, 159,
	156, 168, 82, 50, 255, 192, 204, 18, 131, 221, 253, 162, 94, 232, 28,
	206, 162, 40, 183, 46, 238, 196, 103, 175, 98, 173, 233, 156, 222, 131,
	37, 209, 204, 226, 87, 227, 153, 204, 11, 36, 209, 152, 244, 248, 222,
	88, 203, 138, 204, 67, 95, 118, 238, 211, 13, 87, 130, 136, 240, 125,
	113, 190, 245, 232, 69, 44, 41, 35, 139, 219, 86, 199, 202, 70, 137,
	103, 17, 222, 217, 76, 4, 149, 146, 203, 209, 198, 37, 206, 206, 41, 3,
	119, 159, 180, 99, 66, 203, 50, 175, 17, 108, 181, 166, 128, 44, 154,
	185, 113, 154, 110, 18, 243, 91, 226, 26, 76, 177, 31, 51, 104, 127,
	177, 121, 11, 182, 178, 34, 235, 137, 111, 183, 246, 27, 44, 10, 74,
	142, 230, 70, 242, 110, 38, 162, 209, 225, 22, 217, 185, 126, 207, 69,
	247, 203, 178, 243, 203, 209, 198, 37, 241, 204, 243, 143, 198, 218,
	230, 123, 157, 52, 139, 54, 111, 22, 235, 147, 147, 60, 37, 78, 55, 15,
	197, 44, 209, 218, 199, 227, 59, 70, 23, 27, 69, 88, 150, 248, 107, 177,
	10, 8, 51, 200, 170, 148, 85, 238, 246, 232, 103, 9, 250, 184, 236, 230,
	111, 90, 126, 156, 169, 250, 34, 32, 107, 125, 26, 29, 172, 204, 233,
	84, 92, 19, 78, 45, 172, 222, 200, 26, 188, 71, 189, 162, 139, 238, 54,
	59, 183, 187, 239, 164, 188, 74, 111, 97, 247, 87, 85, 11, 116, 137,
	191, 249, 251, 55, 95, 118, 238, 247, 136, 90, 126, 130, 155, 62, 142,
	89, 61, 213, 213, 231, 170, 147, 115, 189, 234, 249, 236, 220, 175, 95,
	220, 228, 89, 121, 189, 203, 55, 250, 18, 250, 91, 206, 206, 57, 59,
	183, 22, 3, 238, 219, 175, 4, 186, 179, 197, 250, 207, 33, 221, 243, 75,
	60, 41, 175, 64, 81, 131, 151, 240, 123, 92, 222, 163, 245, 191, 62, 45,
	186, 84, 199, 220, 209, 231, 41, 139, 130, 237, 223, 64, 28, 29, 184,
	85, 74, 63, 247, 5, 221, 217, 226, 112, 162, 104, 201, 189, 49, 163, 79,
	154, 123, 139, 242, 59, 7, 240, 172, 151, 100, 231, 90, 15, 116, 211,
	56, 208, 29, 183, 227, 234, 59, 19, 183, 56, 107, 171, 233, 103, 80, 24,
	244, 22, 66, 232, 125, 154, 160, 23, 246, 208, 29, 163, 87, 215, 191,
	209, 139, 243, 47, 202, 13, 164, 87, 251, 77, 138, 67, 152, 116, 204,
	25, 207, 124, 187, 208, 123, 195, 57, 187, 222, 27, 239, 34, 158, 253,
	76, 186, 238, 254, 35, 136, 178, 175, 162, 119, 208, 103, 46, 144, 163,
	206, 189, 164, 230, 110, 253, 28, 176, 139, 171, 62, 220, 180, 231, 68,
	222, 143, 59, 56, 58, 38, 214, 21, 100, 93, 231, 131, 230, 73, 206, 65,
	141, 170, 69, 250, 246, 204, 207, 106, 215, 222, 140, 18, 66, 53, 5,
	183, 25, 108, 252, 0, 11, 211, 158, 10, 154, 160, 189, 18, 172, 84, 75,
	2, 53, 29, 29, 75, 227, 68, 69, 215, 84, 116, 29, 4, 117, 62, 156, 218,
	216, 156, 131, 236, 156, 107, 13, 104, 116, 38, 107, 123, 154, 7, 237,
	23, 147, 19, 121, 229, 254, 209, 119, 246, 223, 63, 90, 15, 202, 35,
	209, 23, 229, 118, 238, 242, 90, 94, 148, 235, 249, 147, 83, 243, 96,
	239, 151, 12, 51, 93, 245, 114, 103, 234, 31, 155, 7, 55, 95, 182, 227,
	169, 105, 190, 94, 67, 191, 111, 119, 90, 19, 128, 226, 90, 18, 114,
	203, 205, 70, 72, 114, 162, 98, 53, 65, 215, 78, 192, 37, 156, 218, 216,
	104, 220, 181, 123, 26, 57, 154, 134, 129, 214, 89, 210, 34, 141, 102,
	171, 29, 97, 80, 24, 20, 121, 72, 17, 151, 141, 210, 168, 122, 124, 255,
	2, 110, 60, 190, 107, 4, 76, 231, 233, 220, 127, 138, 27, 108, 233, 108,
	10, 114, 204, 19, 58, 109, 210, 59, 157, 188, 98, 79, 81, 30, 6, 84,
	123, 196, 211, 48, 8, 3, 125, 227, 146, 110, 45, 118, 199, 238, 73, 24,
	248, 122, 29, 233, 125, 126, 205, 238, 46, 67, 96, 38, 205, 33, 228,
	203, 186, 46, 175, 107, 220, 208, 116, 93, 251, 131, 84, 56, 147, 178,
	181, 134, 44, 106, 172, 135, 153, 150, 92, 99, 45, 55, 44, 177, 194, 48,
	211, 190, 7, 109, 52, 181, 118, 78, 59, 27, 77, 193, 13, 125, 246, 214,
	63, 18, 150, 105, 109, 66, 11, 131, 40, 111, 133, 224, 215, 243, 102,
	176, 236, 164, 218, 11, 182, 66, 32, 236, 106, 71, 103, 142, 250, 244,
	130, 178, 115, 173, 127, 131, 101, 158, 208, 151, 217, 85, 252, 39, 97,
	231, 232, 22, 118, 119, 25, 2, 51, 233, 19, 106, 196, 112, 186, 46, 143,
	218, 172, 174, 74, 232, 211, 115, 72, 133, 115, 21, 55, 11, 143, 103,
	22, 21, 211, 249, 96, 169, 119, 179, 92, 141, 166, 207, 27, 55, 39, 227,
	102, 53, 97, 152, 209, 206, 116, 95, 111, 129, 175, 165, 108, 175, 44,
	119, 163, 189, 199, 51, 60, 205, 34, 58, 209, 176, 81, 18, 112, 166,
	147, 62, 77, 52, 234, 80, 77, 220, 234, 95, 60, 27, 102, 26, 25, 184,
	21, 43, 231, 229, 212, 88, 75, 126, 219, 142, 81, 231, 108, 199, 210,
	171, 171, 163, 24, 27, 108, 209, 171, 243, 251, 229, 202, 215, 235, 31,
	75, 117, 93, 33, 244, 234, 250, 239, 227, 187, 244, 74, 5, 215, 238, 77,
	207, 204, 90, 227, 242, 125, 242, 170, 253, 10, 221, 151, 180, 136, 220,
	10, 179, 136, 178, 134, 162, 164, 162, 220, 106, 66, 158, 80, 173, 83,
	163, 80, 58, 215, 94, 121, 183, 183, 178, 194, 60, 132, 0, 26, 155, 7,
	203, 251, 151, 48, 208, 218, 105, 181, 171, 29, 107, 12, 32, 139, 122,
	24, 233, 61, 187, 62, 134, 219, 53, 214, 125, 25, 102, 50, 249, 227,
	122, 141, 50, 200, 24, 254, 235, 184, 222, 181, 123, 230, 23, 237, 8,
	146, 9, 182, 116, 139, 219, 250, 202, 78, 250, 48, 242, 157, 255, 80,
	85, 91, 255, 77, 186, 223, 73, 53, 10, 241, 109, 49, 225, 18, 157, 16,
	20, 111, 149, 209, 60, 173, 48, 57, 233, 47, 36, 121, 181, 99, 160, 156,
	213, 46, 218, 109, 43, 132, 70, 236, 246, 173, 208, 70, 157, 174, 143,
	225, 245, 47, 151, 248, 165, 152, 201, 4, 175, 115, 94, 19, 252, 25,
	222, 191, 23, 215, 187, 186, 67, 141, 34, 29, 141, 138, 131, 229, 110,
	31, 108, 181, 79, 161, 239, 31, 77, 125, 178, 34, 191, 240, 248, 142,
	181, 158, 55, 173, 144, 80, 204, 190, 139, 153, 219, 113, 111, 225, 171,
	144, 54, 15, 241, 172, 177, 214, 250, 67, 90, 55, 154, 130, 107, 86,
	187, 8, 1, 226, 25, 56, 77, 8, 167, 231, 35, 172, 9, 182, 238, 109, 132,
	107, 170, 112, 183, 176, 187, 175, 102, 120, 183, 141, 235, 93, 221,
	161, 22, 108, 181, 207, 165, 89, 40, 55, 211, 58, 74, 210, 126, 222,
	216, 19, 127, 249, 66, 242, 24, 192, 136, 228, 180, 92, 101, 17, 44, 59,
	202, 117, 92, 48, 204, 6, 75, 159, 230, 240, 26, 147, 87, 141, 250, 238,
	205, 171, 254, 81, 239, 168, 121, 232, 45, 116, 148, 185, 219, 187, 187,
	163, 24, 208, 253, 95, 2, 109, 175, 231, 232, 22, 118, 119, 251, 202,
	10, 211, 117, 113, 189, 171, 59, 63, 171, 93, 115, 95, 226, 182, 118,
	119, 171, 202, 10, 211, 53, 113, 125, 81, 119, 126, 86, 43, 255, 207,
	234, 34, 225, 46, 170, 61, 89, 250, 90, 47, 48, 92, 207, 124, 77, 239,
	215, 119, 193, 79, 228, 255, 236, 126, 26, 105, 110, 214, 84, 83, 77,
	53, 1, 39, 107, 250, 179, 232, 171, 126, 242, 18, 226, 55, 249, 191, 84,
	232, 28, 150, 78, 10, 238, 95, 248, 188, 123, 242, 74, 196, 245, 179,
	198, 186, 177, 126, 124, 39, 162, 250, 215, 106, 183, 219, 239, 246,
	121, 210, 60, 208, 25, 81, 255, 72, 39, 67, 243, 15, 58, 63, 42, 251,
	239, 212, 186, 93, 211, 40, 71, 103, 249, 196, 51, 212, 103, 130, 173,
	84, 58, 210, 57, 242, 34, 170, 29, 255, 223, 216, 1, 4, 129, 38, 197,
	51, 104, 210, 96, 105, 53, 169, 29, 151, 105, 82, 255, 248, 19, 53, 9,
	146, 69, 133, 149, 254, 135, 87, 36, 139, 19, 153, 228, 212, 142, 175,
	169, 179, 127, 79, 130, 38, 209, 137, 163, 95, 147, 232, 92, 64, 107,
	82, 176, 133, 38, 117, 210, 63, 89, 147, 72, 178, 34, 67, 191, 100, 113,
	11, 47, 202, 233, 140, 240, 255, 150, 199, 247, 37, 246, 138, 86, 147,
	248, 222, 150, 95, 147, 158, 55, 101, 154, 52, 255, 248, 170, 38, 105,
	201, 210, 89, 171, 72, 54, 139, 68, 178, 147, 87, 169, 121, 204, 63, 54,
	111, 117, 118, 246, 39, 146, 171, 73, 28, 41, 65, 147, 248, 12, 146, 52,
	137, 206, 18, 69, 178, 79, 147, 118, 76, 245, 86, 34, 72, 22, 213, 214,
	97, 182, 92, 253, 158, 188, 255, 1, 225, 234, 151, 168};

//static constexpr uint8_t WarningCANMessageID = 0x10;
static constexpr uint8_t DashCAN1ID = 0x0E0; //0x11;
static constexpr uint8_t DashCAN2ID = 0x0E1;//0x12;
//static constexpr uint8_t DashCAN3ID = 0x13;
//static constexpr uint8_t DashCAN4ID = 0x14;
//static constexpr uint8_t DashCANInputID = 0x15;

static const CPFECANLib::MSG PROGMEM DashCAN1MSG =
{	{	DashCAN1ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN1Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCAN2MSG =
{	{	DashCAN2ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN2Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCAN3MSG =
{	{	DashCAN3ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN3Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCAN4MSG =
{	{	DashCAN4ID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCAN4Mask = { {0xFFF}, 8, 1, 1, 0};

static const CPFECANLib::MSG PROGMEM DashCANWarningMSG = {
	{	WarningCANMessageID}, 8, 0, 0, 0};

static const CPFECANLib::MSG PROGMEM DashCANWarningMask = { {0xFFF}, 8, 1, 1,
	0};

static const uint8_t ROTARY_MAX_DESC_LENGTH = 32;

class DashLCD {
public:
	virtual ~DashLCD() {
		return;
	}

	enum class OutputState
		: uint8_t {
			Off, Flashing, On
	};

	typedef struct DASHBOARD_DATA {
		uint8_t NDashPage;

		OutputState redLED;
		OutputState yellowLED;
		OutputState greenLED;

		bool waitingForCANOverride;
		uint8_t previousRotaryPositions[CPFERotarySwitch::NUM_ROTARYS];
		bool rotaryOverride;
		CPFERotarySwitch::RotarySwitches rotaryToShow;

	} DASHBOARD_DATA;

	volatile DASHBOARD_DATA DashboardData;

	virtual void updateDashboard() {
		return;
	}

	void TIMER2_OVF_INT() { //Timer 2 OVF INT for CAN Timeout
		++CAN_OVFCount;
		++TransOVFCount;
		++RotaryDispOVFCount;
		++LEDFlashOVFCount;

		if (CAN_OVFCount > CAN_TIMER_OVF_COUNT_MAX) {
			CAN_OVFCount = 0;
			DashboardData.waitingForCANOverride = true;
		}

		if (RotaryDispOVFCount >= ROTARY_DISP_TIMER_OVF_COUNT_MAX) {
			RotaryDispOVFCount = 0;
			DashboardData.rotaryOverride = false;
		}

		if (LEDFlashOVFCount > LED_FLASH_TIMER_OVF_COUNT_MAX) {
			LEDFlashOVFCount = 0;

			if (DashboardData.redLED == OutputState::Flashing) {
				if (PORTB & BIT7)
					PORTB &= ~(BIT7);
				else
					PORTB |= BIT7;
			}
			if (DashboardData.yellowLED == OutputState::Flashing) {
				if (PORTB & BIT6)
					PORTB &= ~(BIT6);
				else
					PORTB |= BIT6;
			}
			if (DashboardData.greenLED == OutputState::Flashing) {
				if (PORTB & BIT5)
					PORTB &= ~(BIT5);
				else
					PORTB |= BIT5;
			}
		}
	}

	void init(CPFECANLib::CAN_MSG_RX canRxIntFunc) {
		Serial.begin(57600);

		if (bootupConfigure()) {
			Serial.println("Init Failed");
		}

		DashboardData.waitingForCANOverride = true;
		waitingForCAN(false); //Faster boot.
		LCD.DisplayOn();

		uploadLogoToController();

		CPFECANLib::init(CPFECANLib::CAN_BAUDRATE::B1M, canRxIntFunc);
		initCAN_RX();

		//Init CAN timeout timer (Timer 2)
		TCCR2A = (1 << CS22) | (1 << CS21) | (1 << CS20); //Normal mode, prescale 1/1024 for 63 OVF/sec
		TIMSK2 = (1 << TOIE2); //Timer 2 OVF Interrupt

		//Init Rotary Switches
		CPFERotarySwitch::init(12);

		//Init Digital Inputs for Buttons
		DDRC &= ~((1 << 4) - 1); //Set direction
		PORTC |= ((1 << 4) - 1); //Enable pullups

		DDRB |= BIT7 + BIT6 + BIT5;
		PORTB &= ~(BIT7 + BIT6 + BIT5);

		DDRF = 0x00;
	}

	void updateDisplay() {
		CPFERotarySwitch::runTasks();

		if (TransOVFCount >= TRANS_TIMER_OVF_COUNT_MAX) {
			TransOVFCount = 0;
			transmitDashboardInfo();
		}

		if (DashboardData.redLED == OutputState::On) {
			PORTB |= BIT7;
		} else if (DashboardData.redLED == OutputState::Off) {
			PORTB &= ~BIT7;
		}

		if (DashboardData.yellowLED == OutputState::On) {
			PORTB |= BIT6;
		} else if (DashboardData.yellowLED == OutputState::Off) {
			PORTB &= ~BIT6;
		}

		if (DashboardData.greenLED == OutputState::On) {
			PORTB |= BIT5;
		} else if (DashboardData.greenLED == OutputState::Off) {
			PORTB &= ~BIT5;
		}

		if (DashboardData.rotaryOverride) {
			rotaryOverride();
		} else if (DashboardData.waitingForCANOverride) {
			waitingForCAN();
		} else {
			updateDashboard();
		}
	}

	void CAN_RX_Int(CPFECANLib::MSG *msg, uint8_t mobNum) {
		resetTimeoutTimer();
		DashboardData.waitingForCANOverride = false;

		switch (msg->identifier.standard) {
		case DashCAN1ID:
			memcpy((void *) &dashCAN1Data, msg->data, sizeof(dashCAN1Data));
			RX_DashCAN1(true);
			break;
		case DashCAN2ID:
			memcpy((void *) &dashCAN2Data, msg->data, sizeof(dashCAN2Data));
			RX_DashCAN2(true);
			break;
		case DashCAN3ID:
			memcpy((void *) &dashCAN3Data, msg->data, sizeof(dashCAN3Data));
			RX_DashCAN3(true);
			break;
		case DashCAN4ID:
			memcpy((void *) &dashCAN4Data, msg->data, sizeof(dashCAN4Data));
			RX_DashCAN4(true);
			break;
		case WarningCANMessageID:
			memcpy((void *) &warningCANData, msg->data, sizeof(warningCANData));
			DashboardData.NDashPage = warningCANData[0];
			RX_WarningCAN(true);
			break;
		}
	}

protected:
	const uint8_t CAN_TIMER_OVF_COUNT_MAX = 254;
	volatile uint8_t CAN_OVFCount;

	const uint8_t TRANS_TIMER_OVF_COUNT_MAX = 12;
	volatile uint8_t TransOVFCount;

	const uint8_t ROTARY_DISP_TIMER_OVF_COUNT_MAX = 60;
	volatile uint8_t RotaryDispOVFCount;

	const uint8_t LED_FLASH_TIMER_OVF_COUNT_MAX = 20;
	volatile uint8_t LEDFlashOVFCount;

	FT801IMPL_SPI LCD;

	static constexpr uint8_t DashCAN1Mob = 0;
	static constexpr uint8_t DashCAN2Mob = 1;
	static constexpr uint8_t DashCAN3Mob = 2;
	static constexpr uint8_t DashCAN4Mob = 3;
	static constexpr uint8_t WarningCANMob = 4;
	static constexpr uint8_t DashCANInputMob = 5;

	typedef struct DashCANInput { //0xF5
		uint8_t ButtonsArray;
		uint8_t RedRotary;
		uint8_t YellowRotary;
		uint8_t BlackRotary;
	} DashCANInput;

	volatile uint8_t dashCAN1Data[8];
	volatile uint8_t dashCAN2Data[8];
	volatile uint8_t dashCAN3Data[8];
	volatile uint8_t dashCAN4Data[8];
	volatile uint8_t warningCANData[8];

	virtual PGM_P blackRotaryString(int position) {
		return 0;
	}
	virtual PGM_P yellowRotaryString(int position) {
		return 0;
	}
	virtual PGM_P redRotaryString(int position) {
		return 0;
	}

	void initCAN_RX() {
		RX_DashCAN1(false);
		RX_DashCAN2(false);
		RX_DashCAN3(false);
		RX_DashCAN4(false);
		RX_WarningCAN(false);
	}

	void transmitDashboardInfo() {
		uint8_t dataBuffer[8];
		DashCANInput *data = (DashCANInput*) dataBuffer;
		CPFECANLib::MSG msg;

		msg.identifier.standard = DashCANInputID;
		msg.data = (uint8_t *) &dataBuffer;
		msg.dlc = 8;
		msg.ide = 0;
		msg.rtr = 0;

		data->ButtonsArray = ~PINC;
		data->BlackRotary = CPFERotarySwitch::getPosition(CPFERotarySwitch::RotarySwitches::BLACK);
		data->YellowRotary = CPFERotarySwitch::getPosition(CPFERotarySwitch::RotarySwitches::YELLOW);
		data->RedRotary = CPFERotarySwitch::getPosition(CPFERotarySwitch::RotarySwitches::RED);

		if (data->BlackRotary != DashboardData.previousRotaryPositions[(uint8_t) CPFERotarySwitch::RotarySwitches::BLACK]) {
			DashboardData.rotaryOverride = true;
			DashboardData.rotaryToShow = CPFERotarySwitch::RotarySwitches::BLACK;
			RotaryDispOVFCount = 0;
		} else if (data->YellowRotary != DashboardData.previousRotaryPositions[(uint8_t) CPFERotarySwitch::RotarySwitches::YELLOW]) {
			DashboardData.rotaryOverride = true;
			DashboardData.rotaryToShow = CPFERotarySwitch::RotarySwitches::YELLOW;
			RotaryDispOVFCount = 0;
		} else if (data->RedRotary != DashboardData.previousRotaryPositions[(uint8_t) CPFERotarySwitch::RotarySwitches::RED]) {
			DashboardData.rotaryOverride = true;
			DashboardData.rotaryToShow = CPFERotarySwitch::RotarySwitches::RED;
			RotaryDispOVFCount = 0;
		}

		for (int i = 0; i < CPFERotarySwitch::NUM_ROTARYS; ++i) {
			DashboardData.previousRotaryPositions[i] = CPFERotarySwitch::getPosition((CPFERotarySwitch::RotarySwitches) i);
		}

		CPFECANLib::sendMsgUsingMOB(DashCANInputMob, &msg);

		//Request Updated Positions for next iteration
		CPFERotarySwitch::requestUpdatedPositions();
	}

	void RX_DashCAN1(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN1Mob, &DashCAN1MSG, &DashCAN1Mask, interruptMode);
	}

	void RX_DashCAN2(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN2Mob, &DashCAN2MSG, &DashCAN2Mask, interruptMode);
	}

	void RX_DashCAN3(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN3Mob, &DashCAN3MSG, &DashCAN3Mask, interruptMode);
	}

	void RX_DashCAN4(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN4Mob, &DashCAN4MSG, &DashCAN4Mask, interruptMode);
	}

	void RX_WarningCAN(bool interruptMode) {
		CPFECANLib::enableMOBAsRX_PROGMEM(WarningCANMob, &DashCANWarningMSG, &DashCANWarningMask, interruptMode);
	}

	int16_t bootupConfigure() {
		LCD.Init(FT_DISPLAY_RESOLUTION, 0, false);
		return 0;
	}

	void uploadLogoToController() {
		LCD.Cmd_Inflate(0);
		LCD.WriteCmdfromflash(CPRacingLogo, sizeof(CPRacingLogo));
		LCD.Finish();
		LCD.DLStart();
		LCD.BitmapHandle(0);
		LCD.BitmapSource(0);
		LCD.BitmapLayout(FT_ARGB1555, 500, 49);
		LCD.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, 250, 49);
		LCD.DLEnd();
		LCD.Finish();
	}

	void waitingForCAN() {
		waitingForCAN(true);
	}

	void waitingForCAN(bool withLogo) {
		LCD.DLStart();
		const char Display_string[] = "Cal Poly FSAE";

		LCD.ColorRGB(0xFF, 0xFF, 0xFF);
		LCD.Cmd_Text(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 8, 29, FT_OPT_CENTER, Display_string);
		LCD.ColorRGB(0xFF, 0x00, 0x00);
		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 29, FT_OPT_CENTER, "WAITING FOR CAN DATA...");

		if (withLogo) {
			LCD.ColorRGB(0xFF, 0xFF, 0xFF);
			LCD.Begin(FT_BITMAPS);
			LCD.Vertex2ii(FT_DISPLAYWIDTH / 4, 215, 0, 0);
			LCD.End();
		}

		LCD.DLEnd();
		LCD.Finish();
	}

	void rotaryOverride() {
		uint32_t color = 0x000000;
		uint8_t position = CPFERotarySwitch::getPosition(DashboardData.rotaryToShow);
		char positionDescription[ROTARY_MAX_DESC_LENGTH];

		switch (DashboardData.rotaryToShow) {
		case CPFERotarySwitch::RotarySwitches::BLACK:
			color = 0x000000;
			strncpy_P(positionDescription, blackRotaryString(position), ROTARY_MAX_DESC_LENGTH);
			break;
		case CPFERotarySwitch::RotarySwitches::YELLOW:
			color = 0xFFFF00;
			strncpy_P(positionDescription, yellowRotaryString(position), ROTARY_MAX_DESC_LENGTH);
			break;
		case CPFERotarySwitch::RotarySwitches::RED:
			color = 0xFF0000;
			strncpy_P(positionDescription, redRotaryString(position), ROTARY_MAX_DESC_LENGTH);
			break;
		}

		LCD.DLStart();
		LCD.ClearColorRGB(color);
		LCD.Clear(1, 1, 1);

		LCD.ColorRGB(~color);
		LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 31, FT_OPT_CENTER, "%d", position);
		LCD.PrintText(25, 25, 31, 0, "%s", positionDescription);

		LCD.DLEnd();
		LCD.Finish();
	}

	float swap(float d) {
		float a;
		unsigned char *dst = (unsigned char *) &a;
		unsigned char *src = (unsigned char *) &d;
		dst[0] = src[3];
		dst[1] = src[2];
		dst[2] = src[1];
		dst[3] = src[0];
		return a;
	}

	uint32_t swap(uint32_t d) {
		uint32_t a;
		unsigned char *dst = (unsigned char *) &a;
		unsigned char *src = (unsigned char *) &d;
		dst[0] = src[3];
		dst[1] = src[2];
		dst[2] = src[1];
		dst[3] = src[0];
		return a;
	}

	uint16_t swap(uint16_t d) {
		uint16_t a;
		unsigned char *dst = (unsigned char *) &a;
		unsigned char *src = (unsigned char *) &d;
		dst[0] = src[1];
		dst[1] = src[0];
		return a;
	}

	void resetTimeoutTimer() {
		CAN_OVFCount = 0;
	}
};
