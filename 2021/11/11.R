res <- as.integer(unlist(strsplit(readLines("11/input2"), "")))
dimY <- length(readLines("11/input2"))
dimX <- nchar(readLines("11/input2")[1])

mat <- t(matrix(res, nrow = dimY, ncol = dimX))

handle <- function(mat, count) {
  fired <- matrix(FALSE, nrow = dimY, ncol = dimX)
  
  doFireCount <- 1
  while (doFireCount > 0) {
    doFireCount <- 0
    
    matNew <- mat
    
    for (y in (1 : dimY)) {
      for (x in (1: dimX)) {
        if (mat[y,x] > 9 && fired[y,x] == FALSE) {
          # FIRE
          fired[y,x] = TRUE
          doFireCount <- doFireCount + 1
          count <- count + 1
          
          if ((x-1) > 0) {
            matNew[y,(x-1)] <- matNew[y,(x-1)] + 1
            if ((y-1) > 0) {
              matNew[(y-1),(x-1)] <- matNew[(y-1),(x-1)] + 1
            }
            if ((y+1) <= dimY) {
              matNew[(y+1),(x-1)] <- matNew[(y+1),(x-1)] + 1
            }
          }
          if ((x + 1) <= dimX) {
            matNew[y,(x+1)] <- matNew[y,(x+1)] + 1
            if ((y-1) > 0) {
              matNew[(y-1),(x+1)] <- matNew[(y-1),(x+1)] +1
            }
            if ((y+1) <= dimY) {
              matNew[(y+1),(x+1)] <- matNew[(y+1),(x+1)] + 1
            }
          }
          if ((y-1) > 0) {
            matNew[(y-1),x] <- matNew[(y-1),x] + 1
          }
          if ((y+1) <= dimY) {
            matNew[(y+1),x] <- matNew[(y+1),x] + 1
          }
        }
        
      }
    }
    
    mat <- matNew
  }
  
  for (y in (1 : dimY)) {
    for (x in (1: dimX)) {
      if (fired[y,x] == TRUE) {
        mat[y,x] = 0
      }
    }
  }
  
  return (list(mat,count))
}

matCpy2 <- mat
mat <- matCpy2
cnt <- 0
tmax <- 1000
t <- 0
atStep <- -1
while (t < tmax) {
  matCpy <- mat
  
  matCpy <- matCpy + 1
  
  resu <- handle(matCpy, cnt)
  matCpy <- resu[[1]]
  cnt2 <- resu[[2]]
  
  if ((cnt2 - cnt) == (dimY * dimX)) {
    atStep <- t
    break
  }
  
  mat <- matCpy
  t <- t + 1
}


