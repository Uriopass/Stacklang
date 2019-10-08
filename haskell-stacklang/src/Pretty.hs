module Pretty where

class Pretty a where
    showP :: a -> String

pPrint :: (Pretty a) => a -> IO ()
pPrint = putStrLn . showP


instance (Pretty a, Pretty b) => Pretty (a, b) where
  showP (x, y) = showP x <> " " <> showP y

instance (Pretty a, Pretty b) => Pretty (Either a b) where
  showP (Left  a) = showP a
  showP (Right a) = showP a

instance Pretty () where
  showP = const ""
