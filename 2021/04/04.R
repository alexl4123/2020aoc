# 04.12.2021
res <- readLines("04/ex_2_02.csv")

firstLine <- unlist(strsplit(res[1], ","))

input_to_matrices <- function(res) {
  matrices <- list()
  curListIndex <- 1
  curMat <- c()
  for (i in (2:length(res))) {
    if (res[i] == "") {
      if (length(curMat) > 0) {
        test2 <- unlist(strsplit(curMat[[1]], " "))
        test2 <- test2[test2 != ""]
        test <- unlist(strsplit(curMat, " "))
        test <- test[test != ""]
        curMatrix <- matrix(test, ncol = length(test2), byrow = TRUE)
        matrices[[curListIndex]] <- curMatrix
        curListIndex <- curListIndex + 1
      }
      curMat <- c()
    } else {
      curMat <- c(curMat, res[i])
    }
  }
  return (matrices)
}

all_items_same <- function(vector, item = "X") {
    return (Reduce(function(cum, cur) {return (cum && (cur == item))}, vector, TRUE))
}


exercise1 <- function(matrices) {
  winner <- FALSE
  for (x in firstLine) {
    for (i in c(1:length(matrices))) {
      hit <- match(x, matrices[[i]])
      if (!is.na(hit)) {
        matrices[[i]][hit] <- "X"
      }
    }
  
    for (i in c(1:length(matrices))) {
      res_m1 <- apply(matrices[[i]], MARGIN = 1, FUN = all_items_same)
      hit_m1 <- match(TRUE, res_m1)
      res_m2 <- apply(matrices[[i]], MARGIN = 2, FUN = all_items_same)
      hit_m2 <- match(TRUE, res_m2)
      
      if (!is.na(hit_m1)) {
        # This board won
        s <- sum(as.integer(matrices[[i]][matrices[[i]] != "X"]))
        res1 <- s * as.integer(x)
        winner <- TRUE
        break
      } else if(!is.na(hit_m2)) {
        # This board won
        s <- sum(as.integer(matrices[[i]][matrices[[i]] != "X"]))
        res1 <- s * as.integer(x)
        winner <- TRUE
        break
      }
    }
    
    if(winner) {
      break
    }
  } 
  
  return (res1)
}

matrices <- input_to_matrices(res)
res1 <- exercise1(matrices)

exercise2 <- function(matrices) {
  winner <- c()
  for (x in firstLine) {
    for (i in c(1:length(matrices))) {
      hit <- match(x, matrices[[i]])
      if (!is.na(hit)) {
        matrices[[i]][hit] <- "X"
      }
    }
  
    for (i in c(1:length(matrices))) {
      res_m1 <- apply(matrices[[i]], MARGIN = 1, FUN = all_items_same)
      hit_m1 <- match(TRUE, res_m1)
      res_m2 <- apply(matrices[[i]], MARGIN = 2, FUN = all_items_same)
      hit_m2 <- match(TRUE, res_m2)
      
      if (!is.na(hit_m1) && is.na(match(i, winner))) {
        print(paste("won",i,x))
        winner <- union(winner, i)
    } else if(!is.na(hit_m2) && is.na(match(i,winner))) {
        # This board won
        print(paste("won",i,x))
        winner <- union(winner, i)
      }
    }
    
    if (length(winner) >= length(matrices)) {
      print(paste("fin",x)) 
      win_board <- winner[length(winner)]
      s <- sum(as.integer(matrices[[win_board]][matrices[[win_board]] != "X"]))
      res2 <- s * as.integer(x)
      break
    }
  } 
 
  
  return (res2)
}

matrices <- input_to_matrices(res)
res2 <- exercise2(matrices)
