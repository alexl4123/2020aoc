# 07.12.
res <- as.integer(unlist(strsplit(readLines("07/ex_1_02.csv"), ",")))
initial_guess <- median(res)
res1 <- sum(abs(res - initial_guess))

# exercise 2
cost <- function(data) {
  return (sum(sapply(data, function(x) {sum(c(1:x))})))
}
# start at the mean (is basically answer)
start <- floor(mean(res))
helper2 <- cost(abs(res - start))

# Basically a hill climbing algorithm (just with valleys)
fin <- FALSE
while(!fin) {
  pos1 <- start - 1
  helper2_1 <- cost(abs(res - pos1))
  
  pos2 <- start + 1
  helper2_2 <- cost(abs(res - pos2))
  
  if(helper2_1 < helper2) {
    start <- pos1
    helper2 <- helper2_1
  } else if (helper2_2 < helper2) {
    start <- pos2
    helper2 <- helper2_2
  } else {
    fin <- TRUE
  }
}

res2.pos <- start
res2 <- helper2




