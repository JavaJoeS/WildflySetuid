package com.daggerpoint.share;

public class SharedDaggerpoint {

		static {
			    System.loadLibrary("SharedLibrary");
		}
		public native void greetings();
		public static void main(String[] args) {
			new SharedDaggerpoint().greetings();
		}
}
