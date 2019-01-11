/*
    COP - Codes of OPerations

    This file is part of YLA VM (Yet another Language for Academic purpose: Virtual Machine).

    YLA VM is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef _yla_cop_h
#define _yla_cop_h

/*
codes of operations
*/
#define CNOP	0x00

#define CPUSH	0x01
#define CDUP	0x02
#define CDEEP	0x03
#define CSTK	0x04
#define CGDUP	0x0a
#define CGDEEP	0x0b
#define CGSTK	0x0c

#define CTEST	0x05
#define CCMP	0x06

#define CLOAD	0x08
#define CSAVE	0x09

#define CADD	0x10
#define CSUB	0x11
#define CMULT	0x12
#define CDIV	0x13
#define CNEG	0x14

#define CITOA	0x15

#define CJMP	0x20
#define CJZ		0x21
#define CJNZ	0x22
#define CJE		0x23
#define CJNE	0x24

#define CAL		0x40
#define CALO	0x41
#define CRET	0x42

#define CIN		0x30
#define CINS	0x31
#define COUT	0x32

#define CHALT	0xff

#endif
