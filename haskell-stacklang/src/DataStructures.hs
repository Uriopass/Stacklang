{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE ConstraintKinds #-}

module DataStructures where

import           Pretty
import qualified Data.Map                      as Map
import           Control.Monad.Except
import           Control.Monad.State
import           Control.Monad.Reader

data Atom = Number Int | String String | Identifier String | Declaration String
    deriving (Show)

data Expr = Atom Atom | List [Expr]
    deriving (Show)

type Stack = [Expr]
type Variables = Map.Map String Expr

type MonadEval m = (MonadError StacklangError m, MonadIO m, MonadState Stack m, MonadReader Variables m)

data StacklangError
    = ParseError String
    | NoSuchFunction String
    | StackTooSmall Stack
    | TypeError String Expr
    | MiscError String
    deriving (Show)

instance Pretty Atom where
  showP (Number      v) = show v
  showP (String      s) = show s
  showP (Identifier  s) = s
  showP (Declaration s) = "/" ++ s

instance Pretty Expr where
  showP (List v) = "{" ++ unwords (showP <$> v) ++ "}"
  showP (Atom a) = showP a

instance Pretty Stack where
  showP v = "[" ++ unwords (showP <$> v) ++ "]"

instance Pretty StacklangError where
  showP (ParseError     s) = "parse error: " ++ s
  showP (NoSuchFunction s) = "no such function: " ++ s
  showP (StackTooSmall  s) = "not enough values on the stack " ++ showP s
  showP (MiscError      s) = s
  showP (TypeError s e   ) = "function `" ++ s ++ "` has invalid arg " ++ show e

