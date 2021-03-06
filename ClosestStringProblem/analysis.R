# Analysis Swarm Intelligence project
# By Arno Moonens

args = commandArgs(trailingOnly=TRUE)
if (length(args)<2) {
  stop("2 arguments needed. Usage: Rscript analysis.R PATH/TO/RESULTS/ PATH/TO/instances_opt.txt", call.=FALSE)
}

results.folder <- args[1]
optimals <- read.table(file=args[2], header=TRUE, sep=";", row.names = "filename") 
row.names(optimals) <- sapply(row.names(optimals), function(x) substr(x, 0, nchar(x) - 4))
optimals <- optimals[order(row.names(optimals)),] # sort by instance name

upb <- optimals[,"ub"]

mmas.results.file <- paste0(results.folder, "/mmas.txt")
mmas.ls.results.file <- paste0(results.folder, "/mmas-ls.txt")
acs.results.file <- paste0(results.folder, "/acs.txt")

process.file <- function(f) {
  res <- read.csv(f, header = FALSE)
  colnames(res) <- c("Instance", 1:10)
  rownames(res) <- res$Instance
  res$Instance <- NULL
  res[order(row.names(res)),] # sort by instance name
}

mmas.results <- process.file(mmas.results.file)
mmas.ls.results <- process.file(mmas.ls.results.file)
acs.results <- process.file(acs.results.file)


algo.stats <- function(results) {
  res <- data.frame(
    instance=rownames(results),
    B=apply(results, 1, min),
    W=apply(results, 1, max),
    M=apply(results, 1, mean),
    SD=apply(results, 1, sd)
  )
  rownames(res) <- NULL
  return(res)
}
# Best (B), worst (W), mean (M) and standard deviation (SD) for both algorithms
mmas.stats <- algo.stats(mmas.results)
write.csv(mmas.stats, paste0(results.folder, "/mmas-results.csv"), row.names = FALSE)

mmas.ls.stats <- algo.stats(mmas.ls.results)
write.csv(mmas.ls.stats, paste0(results.folder, "/mmas-ls-results.csv"), row.names = FALSE)

acs.stats <- algo.stats(acs.results)
write.csv(acs.stats, paste0(results.folder, "/acs-results.csv"), row.names = FALSE)

rpd <- function(x) ((x - upb) * 100) / upb

# Relative percentage deviation (between mean and upper best) for both algorithms
mmas.deviations <- apply(X = mmas.results, 2, rpd)
acs.deviations <- apply(X = acs.results, 2, rpd)

deviations <- data.frame(
  instance=rownames(mmas.deviations),
  MMAS=apply(mmas.deviations, 1, mean),
  ACS=apply(acs.deviations, 1, mean)
)
write.csv(format(deviations, digits=3), paste0(results.folder, "/rpd.csv"), row.names = FALSE, quote= FALSE)

mmas.deviations.flat <- c(mmas.deviations, recursive = TRUE)
acs.deviations.flat <- c(acs.deviations, recursive = TRUE)
boxplots <- boxplot(mmas.deviations.flat, acs.deviations.flat, names=c("MMAS", "ACS"))

cat("MMAS and ACS statistics:")
print(boxplots$stats)

cat("Medians of MMAS and ACS", boxplots$stats[3,], "\n")

cat("Wilcoxon rank sum test for MMAS and ACS")
print(wilcox.test(unlist(mmas.results), unlist(acs.results), paired = T))

# Convergence
plot.convergence <- function(algo, instance, ylim) {
  data <- data.frame(row.names = c(1:1000)) # assume 1000 iterations
  for(run in c(1:10)) {
    run.data <- read.table(paste0(results.folder, "/", algo, "/", instance, "-", run, ".txt"), fill = T)
    run.data <- run.data[complete.cases(run.data),]
    run.data <- merge(expand.grid(V1 = 1:1000), run.data, all = T)
    rownames(run.data) <- run.data$V1
    run.data$V1 <- NULL
    colnames(run.data) <- c("cost")
    # Fill solution qualities for iterations without improvement.
    # Source: http://www.cookbook-r.com/Manipulating_data/Filling_in_NAs_with_last_non-NA_value/
    goodIdx <- !is.na(run.data)
    goodVals <- c(NA, run.data[goodIdx])
    fillIdx <- cumsum(goodIdx)+1
    run.data$cost <- goodVals[fillIdx]
    data[run] <- run.data$cost
  }
  means <- apply(data, 1, mean)
  plot(rownames(data), means, type = "l", xlab="Iteration", ylab="Cost", ylim = ylim)
}
instance <- "2-30-10000-1-9"
ylim <- c(4290, 4470)
plot.convergence("mmas", instance, ylim)
plot.convergence("acs", instance, ylim)

## Local search
mmas.ls.deviations <- apply(X = mmas.ls.results, 2, rpd)

deviations.mmas.ls <- data.frame(
  instance=rownames(mmas.deviations),
  "MMAS"=apply(mmas.deviations, 1, mean),
  "MMAS LS"=apply(mmas.ls.deviations, 1, mean)
)
write.csv(format(deviations.mmas.ls, digits=3), paste0(results.folder, "/rpd-ls.csv"), row.names = FALSE, quote= FALSE)

mmas.ls.deviations.flat <- c(mmas.ls.deviations, recursive = TRUE)
boxplots.ls <- boxplot(mmas.deviations.flat, mmas.ls.deviations.flat, names=c("MMAS", "MMAS local search"))

cat("MMAS and MMAS with LS statistics:")
print(boxplots.ls$stats)

cat("Medians of MMAS and MMAS with LS", boxplots.ls$stats[3,], "\n")

# Wilcoxon test
cat("Wilcoxon rank sum test for MMAS and MMAS with local search")
print(wilcox.test(unlist(mmas.results), unlist(mmas.ls.results), paired = T))

ylim=c(4280,4470)
plot.convergence("mmas", instance, ylim)
plot.convergence("mmas-ls", instance, ylim)
