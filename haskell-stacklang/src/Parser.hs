{-# OPTIONS_GHC -Wno-unused-do-bind #-}

module Parser where

import           Text.ParserCombinators.Parsec     hiding ( spaces )
import           DataStructures

comment :: Parser ()
comment = do
    char '#'
    skipMany (noneOf "\n")
    optional spaces

spaces :: Parser ()
spaces = comment <|> (skipMany1 space >> optional comment)

number :: Parser Atom
number = Number . read <$> many1 digit

stringP :: Parser Atom
stringP = do
  char '"'
  x <- many (noneOf "\"")
  char '"'
  return $ String x

identifier :: Parser String
identifier = do
  x <- noneOf "\"\n\t {}1234567890/"
  y <- many (noneOf "\"\n\t {}/")
  return $ x : y

declaration :: Parser Atom
declaration = do
  char '/'
  Declaration <$> identifier

atom :: Parser Expr
atom = Atom <$> (number <|> stringP <|> (Identifier <$> identifier) <|> declaration)

instruction :: Parser Expr
instruction = do
  optional spaces
  x <- expr
  lookAhead (space <|> char '}' <|> char '#')
  optional spaces
  return x

list :: Parser Expr
list = do
  char '{'
  x <- many instruction
  char '}'
  return $ List x

expr :: Parser Expr
expr = atom <|> list

parseAll :: String -> Either ParseError Expr
parseAll = parse (expr <* eof) "Stacklang"
