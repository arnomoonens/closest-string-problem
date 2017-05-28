
# Uncomment to use in RStudio
commandArgs <- function (trailingOnly=FALSE) {
  c("/Users/arnomoonens/Dropbox/MA2-AI/Swarm_Intelligence/project/results", "/Users/arnomoonens/Dropbox/MA2-AI/Swarm_Intelligence/project/instances/instances_opt.txt")
}

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
acs.results.file <- paste0(results.folder, "/acs.txt")

mmas.results <- read.csv(mmas.results.file, header = FALSE)
colnames(mmas.results) <- c("Instance", 1:10)
rownames(mmas.results) <- mmas.results$Instance
mmas.results$Instance <- NULL
mmas.results <- mmas.results[order(row.names(mmas.results)),] # sort by instance name

acs.results <- read.csv(acs.results.file, header = FALSE)
colnames(acs.results) <- c("Instance", 1:10)
rownames(acs.results) <- acs.results$Instance
acs.results$Instance <- NULL
acs.results <- acs.results[order(row.names(acs.results)),] # sort by instance name

# Best (B), worst (W), mean (M) and standard deviation (SD) for both algorithms
mmas.stats <- data.frame(
  instance=rownames(mmas.results),
  B=apply(mmas.results, 1, min),
  W=apply(mmas.results, 1, max),
  M=apply(mmas.results, 1, mean),
  SD=apply(mmas.results, 1, sd)
  )
rownames(mmas.stats) <- NULL
write.csv(mmas.stats, paste0(results.folder, "/mmas-results.csv"), row.names = FALSE)

acs.stats <- data.frame(
  instance=rownames(acs.results),
  B=apply(acs.results, 1, min),
  W=apply(acs.results, 1, max),
  M=apply(acs.results, 1, mean),
  SD=apply(acs.results, 1, sd)
)
rownames(acs.stats) <- NULL
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
  
# Wilcoxon test
wilcox.test(mmas.stats[,"M"], acs.stats[,"M"])
