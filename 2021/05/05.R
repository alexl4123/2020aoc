# 05.12.2021
res <- readLines("05/ex_1_02.csv")

res <- sapply(res, function(x) {strsplit(x, "->")})

res2 <- sapply(res, function(x) {
  cur <- c(unlist(strsplit(x[1], ",")), unlist(strsplit(x[2], ",")))
  return (cur)
  })
mat <- (t(as.matrix(res2)))
mat <- matrix(as.integer(mat), ncol = 4)

mat <- mat + 1 # because of R 

mat.dimx <- max(mat[,1], mat[,3])
mat.dimy <- max(mat[,2], mat[,4])

# ex1 
ex1.indices <- union(which(mat[,2] == mat[,4]), which(mat[,1] == mat[,3]))
ex1.mat <- mat[ex1.indices, ]

matrices <- list()
matrices.index <- 1
for (index in c(1:length(ex1.mat[,1]))) {
  x <- ex1.mat[index,]
  ret_mat <- matrix(0, nrow = mat.dimy, ncol = mat.dimx)
  if (x[1] == x[3]) {
    if (x[2] > x[4]) {
      tmp <- x[2]
      x[2] <- x[4]
      x[4] <- tmp
    }
    for (i in c(x[2]: x[4])) {
      ret_mat[i, x[1]] <- 1
    }
  } else if (x[2] == x[4]) {
    if (x[1] > x[3]) {
      tmp <- x[1]
      x[1] <- x[3]
      x[3] <- tmp
    }
    for (i in c(x[1]:x[3])) {
      ret_mat[x[2], i] <- 1
    }
  }
  matrices[[matrices.index]] <- ret_mat
  matrices.index <- matrices.index + 1
}

res_mat <- matrix(0, nrow = mat.dimy, ncol = mat.dimx)
for (i in c(1:length(matrices))) {
  res_mat <- res_mat + matrices[[i]]
}
res1 <- sum(res_mat > 1)

#ex2
ex2.indices <- which((abs(mat[,1] - mat[,3]) / abs(mat[,2] - mat[,4])) == 1)
ex2.mat <- mat[ex2.indices, ]

matrices <- list()
matrices.index <- 1
for (index in c(1:length(ex2.mat[,1]))) {
  x <- ex2.mat[index,]
  ret_mat <- matrix(0, nrow = mat.dimy, ncol = mat.dimx)
  if (((x[1] > x[3]) && (x[2] > x[4]))
      || ((x[1] < x[3]) && (x[2] < x[4]))) {
    if (x[1] > x[3]) {
      tmp1 <- x[1]
      tmp2 <- x[2]
      
      x[1] <- x[3]
      x[2] <- x[4]
      
      x[3] <- tmp1
      x[4] <- tmp2
    }
    for (i in c(0 : (x[4] - x[2]))) {
      ret_mat[x[2] + i, x[1] + i] <- 1
    }
  } else if(((x[1] > x[3]) && (x[2] < x[4])) ||
            ((x[1] < x[3]) && (x[2] > x[4]))) {
     if (x[1] > x[3]) {
      tmp1 <- x[1]
      tmp2 <- x[2]
      
      x[1] <- x[3]
      x[2] <- x[4]
      
      x[3] <- tmp1
      x[4] <- tmp2
    }
    for (i in c(0 : (x[3] - x[1]))) {
      ret_mat[x[2] - i, x[1] + i] <- 1
    }   
  }
  matrices[[matrices.index]] <- ret_mat
  matrices.index <- matrices.index + 1
}

res_mat <- res_mat #from ex1
for (i in c(1:length(matrices))) {
  res_mat <- res_mat + matrices[[i]]
}
res2 <- sum(res_mat > 1)









