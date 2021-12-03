# 03.12.2021
res <- readLines("03/ex_2_02.csv")

n <- nchar(res[1])
powers <- 2 ** (seq(n-1,0,-1))

res <- unlist(strsplit(res, split = ""))

mat <- matrix(data = res, ncol = n, byrow = TRUE)

det_up <- function(mat) {
 up <- apply(mat, MARGIN = 2, FUN = function(x) {
  tab <- table(x)
  if (length(names(tab)) == 1) {
    if (grepl(pattern = "0", names(tab))) {
      return (0)
    } else {
      return (1)
    }
  } else {
    if (tab[names(tab) == "0"] > tab[names(tab) == "1"]) {
      return (0)
    } else {
      return (1)
    }  
  }
  })
 return (up)
}

binToDec <- function(up, powers) {
  return (c(up %*% powers))
}

up <- det_up(mat)
gamma <- binToDec(up, powers)
epsilon <- binToDec(sapply(up, function(x) {if (x == 0) {return(1)} else {return(0)}}), powers)
res1 <- gamma * epsilon

## Ex 2:

exercise2 <- function(mat, isOxygen){
  cur_indices <- c(1:length(mat[,1]))  
  for (i in c(1:n)) {
    cur_up <- det_up(mat[cur_indices,])
    if (isOxygen) {
      cur_indices_2 <- which(mat[,i] == cur_up[i])
    } else {
      cur_indices_2 <- which(mat[,i] != cur_up[i])
    }
    cur_indices <- intersect(cur_indices, cur_indices_2)
    
    if (length(cur_indices) == 1) {
      break;
    }
  }
  
  return (cur_indices)
}

oxygen <- binToDec(as.integer(mat[exercise2(mat, TRUE),]), powers)
co2 <- binToDec(as.integer(mat[exercise2(mat, FALSE), ]), powers)
res2<- oxygen * co2

