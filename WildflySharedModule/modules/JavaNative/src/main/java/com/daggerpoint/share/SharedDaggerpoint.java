package com.daggerpoint.share;

public class SharedDaggerpoint {

		static {
			    System.loadLibrary("SharedLibrary");
		}
		public SharedDaggerpoint daggerPtr;
		public native void greetings();
		public static void main(String[] args) {
			SharedDaggerpoint dagger = new SharedDaggerpoint();
			dagger.greetings();
		}
}
