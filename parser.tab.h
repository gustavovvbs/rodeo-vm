/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IF = 258,
     ELSE = 259,
     WHILE = 260,
     SPEED = 261,
     TORQUE = 262,
     YAW = 263,
     BRAKE = 264,
     WAIT = 265,
     PATTERN = 266,
     READ = 267,
     CALM = 268,
     SWIRL = 269,
     AGGRESSIVE = 270,
     RIDER = 271,
     TILT = 272,
     RPM = 273,
     EMERGENCY = 274,
     TIME_MS = 275,
     EQ = 276,
     NE = 277,
     GE = 278,
     LE = 279,
     GT = 280,
     LT = 281,
     ASSIGN = 282,
     PLUS = 283,
     MINUS = 284,
     MULT = 285,
     DIV = 286,
     LPAREN = 287,
     RPAREN = 288,
     LBRACE = 289,
     RBRACE = 290,
     SEMICOLON = 291,
     ARROW = 292,
     IDENTIFIER = 293,
     NUMBER = 294
   };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define WHILE 260
#define SPEED 261
#define TORQUE 262
#define YAW 263
#define BRAKE 264
#define WAIT 265
#define PATTERN 266
#define READ 267
#define CALM 268
#define SWIRL 269
#define AGGRESSIVE 270
#define RIDER 271
#define TILT 272
#define RPM 273
#define EMERGENCY 274
#define TIME_MS 275
#define EQ 276
#define NE 277
#define GE 278
#define LE 279
#define GT 280
#define LT 281
#define ASSIGN 282
#define PLUS 283
#define MINUS 284
#define MULT 285
#define DIV 286
#define LPAREN 287
#define RPAREN 288
#define LBRACE 289
#define RBRACE 290
#define SEMICOLON 291
#define ARROW 292
#define IDENTIFIER 293
#define NUMBER 294




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 18 "parser.y"
{
    int number;
    char *string;
    Expression *expr;
    Condition *cond;
    ASTNode *stmt;
    ASTNode *stmt_list;
    BinaryOp binop;
    RelOp relop;
    Pattern pattern;
    SensorType sensor;
}
/* Line 1529 of yacc.c.  */
#line 140 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

