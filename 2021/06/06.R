# 06.12.2021
res <- as.integer(unlist(strsplit(readLines("06/ex_1_02.csv"), split = ",")))
mat <- matrix(0, nrow = 9, ncol = 2)
colnames(mat) <- c("date", "amount")
rownames(mat) <- c(0:8)
mat[,1] <- c(0:8)

items <- unlist(table(res))
items <- c(0, items, rep.int(0, 8 - length(items))) # 0 not in input data
mat[,2] <- items

for (i in c(1:256)) {
  mat[,1] <- mat[,1] - 1
  index_1 <- which(mat[,1] < 0)
  index_2 <- which(mat[,1] == 6)
  
  mat[index_2,2] <- mat[index_2,2] + mat[index_1,2]
  mat[index_1,1] <- 8
}

res1 <- sum(mat[,2])

