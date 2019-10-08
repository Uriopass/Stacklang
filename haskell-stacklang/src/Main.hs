{-# LANGUAGE FlexibleContexts #-}
module Main where

import           Control.Monad.Except
import           Parser
import           Interpreter
import           DataStructures
import qualified Data.Map                      as Map
import           Pretty
import           Control.Monad.State.Lazy                 ( StateT
                                                          , runStateT
                                                          )
import           Control.Monad.Reader                     ( ReaderT
                                                          , runReaderT
                                                          )

parseString :: (MonadError StacklangError m) => String -> m Expr
parseString input = either (throwError . ParseError . show) return (parseAll input)

type Run = StateT Stack (ReaderT Variables (ExceptT StacklangError IO))

runProgram :: String -> Run ()
runProgram toParse = do
  x <- parseString toParse
  push x
  exec

main :: IO ()
main = do
  c <- getContents
  let toParse = "{ " ++ c ++ "  }"
  print toParse
  let a = runStateT (runProgram toParse) []
  let b = runReaderT a (Map.fromList [])
  let d = runExceptT b
  result <- d
  pPrint result
  return ()
